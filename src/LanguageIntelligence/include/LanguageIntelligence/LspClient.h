// include/LanguageIntelligence/LspClient.h
#pragma once

#include "Diagnostic.h"
#include "LspMessage.h"
#include <Core/Types.h>
#include <Platform/Path.h>
#include <Editor/Document.h>
#include <functional>
#include <memory>
#include <vector>

namespace CodeWizard::LanguageIntelligence {

class LspClient {
public:
    using DiagnosticsCallback = std::function<void(const std::vector<Diagnostic>&)>;

    explicit LspClient(const Platform::Path& executablePath);
    ~LspClient();

    [[nodiscard]] bool start();
    void stop();

    void initialize(const Platform::Path& rootPath);
    void openDocument(const Editor::Document& document);
    void closeDocument(const Platform::Path& filePath);
    void changeDocument(const Editor::Document& document, const Editor::Change& change);

    void setDiagnosticsCallback(DiagnosticsCallback callback);

private:
    void readFromProcess();
    void writeMessage(const LspMessage& message);
    void handleNotification(const LspMessage& msg);
    void handleResponse(const LspMessage& msg);

    Platform::Path m_executablePath;
    FILE* m_readPipe = nullptr;
    FILE* m_writePipe = nullptr;
    pid_t m_pid = 0;
    bool m_isInitialized = false;
    DiagnosticsCallback m_diagnosticsCallback;
    LspMessageParser m_parser;
    int m_nextId = 1;
};

} // namespace CodeWizard::LanguageIntelligence
