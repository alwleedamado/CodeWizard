// src/LspClient.cpp
#include "LanguageIntelligence/LspClient.h"
#include <Core/Logger.h>
#include <Services/GlobalServices.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <thread>
#include <chrono>

using namespace CodeWizard::Core;

namespace CodeWizard::LanguageIntelligence {

LspClient::LspClient(const Platform::Path& executablePath) : m_executablePath(executablePath) {}

LspClient::~LspClient() {
    stop();
}

bool LspClient::start() {
    if (m_pid != 0) return true; // Already running

    int toChild[2];   // Parent -> child
    int fromChild[2]; // Child -> parent

    if (pipe(toChild) != 0 || pipe(fromChild) != 0) {
        CW_LOG_ERROR("Failed to create pipes for LSP");
        return false;
    }

    m_pid = fork();
    if (m_pid == 0) {
        // Child process
        dup2(toChild[0], STDIN_FILENO);
        dup2(fromChild[1], STDOUT_FILENO);
        close(toChild[0]);
        close(toChild[1]);
        close(fromChild[0]);
        close(fromChild[1]);

        execl(m_executablePath.native().c_str(), m_executablePath.native().c_str(), "--log=error", nullptr);
        _exit(1);
    } else if (m_pid > 0) {
        // Parent process
        close(toChild[0]);
        close(fromChild[1]);
        m_writePipe = fdopen(toChild[1], "w");
        m_readPipe = fdopen(fromChild[0], "r");

        // Make read pipe non-blocking
        int flags = fcntl(fileno(m_readPipe), F_GETFL, 0);
        fcntl(fileno(m_readPipe), F_SETFL, flags | O_NONBLOCK);

        // Start reader thread
        std::thread([this]() {
            while (m_pid > 0) {
                readFromProcess();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }).detach();

        return true;
    } else {
        CW_LOG_ERROR("Failed to fork LSP process");
        close(toChild[0]);
        close(toChild[1]);
        close(fromChild[0]);
        close(fromChild[1]);
        return false;
    }
}

void LspClient::stop() {
    if (m_pid == 0) return;

    if (m_writePipe) {
        fclose(m_writePipe);
        m_writePipe = nullptr;
    }

    // Wait for child to exit
    int status;
    waitpid(m_pid, &status, 0);
    m_pid = 0;

    if (m_readPipe) {
        fclose(m_readPipe);
        m_readPipe = nullptr;
    }
}

void LspClient::initialize(const Platform::Path& rootPath) {
    if (!m_isInitialized) {
        LspMessage msg;
        msg.method = "initialize";
        msg.id = m_nextId++;
        nlohmann::json params;
        params["processId"] = getpid();
        params["rootUri"] = "file://" + rootPath.native();
        params["capabilities"] = nlohmann::json::object();
        msg.params = params;
        writeMessage(msg);
        m_isInitialized = true;
    }
}

void LspClient::setDiagnosticsCallback(DiagnosticsCallback callback) {
    m_diagnosticsCallback = std::move(callback);
}

void LspClient::readFromProcess() {
    if (!m_readPipe) return;

    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), m_readPipe)) > 0) {
        m_parser.appendData(std::string_view(buffer, bytesRead));
        while (m_parser.hasMessage()) {
            try {
                LspMessage msg = m_parser.takeMessage();
                if (msg.id == -1) {
                    handleNotification(msg);
                } else {
                    handleResponse(msg);
                }
            } catch (const std::exception& e) {
                CW_LOG_WARN("Failed to parse LSP message: " + std::string(e.what()));
            }
        }
    }
}

void LspClient::writeMessage(const LspMessage& message) {
    if (!m_writePipe) return;

    nlohmann::json json;
    json["jsonrpc"] = "2.0";
    if (message.id != -1) {
        json["id"] = message.id;
    }
    json["method"] = message.method;
    json["params"] = message.params;

    std::string jsonString = json.dump();
    std::string header = "Content-Length: " + std::to_string(jsonString.size()) + "\r\n\r\n";
    fwrite(header.c_str(), 1, header.size(), m_writePipe);
    fwrite(jsonString.c_str(), 1, jsonString.size(), m_writePipe);
    fflush(m_writePipe);
}

void LspClient::handleNotification(const LspMessage& msg) {
    if (msg.method == "textDocument/publishDiagnostics") {
        std::vector<Diagnostic> diagnostics;
        auto uri = msg.params.value("uri", "");
        auto diags = msg.params.value("diagnostics", nlohmann::json::array());

        for (const auto& diagJson : diags) {
            Diagnostic diag;
            diag.severity = static_cast<DiagnosticSeverity>(diagJson.value("severity", 2));
            diag.message = diagJson.value("message", "");
            diag.source = diagJson.value("source", "lsp");

            auto range = diagJson.value("range", nlohmann::json{});
            auto start = range.value("start", nlohmann::json{});
            auto end = range.value("end", nlohmann::json{});

            diag.range.start.line = start.value("line", 0u);
            diag.range.start.character = start.value("character", 0u);
            diag.range.end.line = end.value("line", 0u);
            diag.range.end.character = end.value("character", 0u);

            diagnostics.push_back(std::move(diag));
        }

        if (m_diagnosticsCallback) {
            m_diagnosticsCallback(diagnostics);
        }
    }
}

void LspClient::handleResponse(const LspMessage& msg) {
    if (msg.method.empty() && msg.id == 1) {
        // Initialize response - send initialized notification
        LspMessage initialized;
        initialized.method = "initialized";
        initialized.params = nlohmann::json::object();
        writeMessage(initialized);
    }
}

} // namespace CodeWizard::LanguageIntelligence
