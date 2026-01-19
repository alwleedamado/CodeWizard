// src/BuildSystem.cpp
#include "BuildRun/BuildSystem.h"
#include <Services/GlobalServices.h>
#include <array>
#include <cstdio>
#include <memory>

namespace CodeWizard::BuildRun {

BuildSystem::BuildSystem(const Platform::Path& workingDir) : m_workingDir(workingDir) {}

Result<BuildResult> BuildSystem::buildSync() {
    std::array<char, 128> buffer;
    std::string result;
    std::string command = "cd " + m_workingDir.native() + " && make 2>&1";

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        return failure<BuildResult>(Core::ErrorCode::IoError, "Failed to run build command");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    BuildResult buildResult;
    buildResult.success = (pclose(pipe.release()) == 0);
    buildResult.output = std::move(result);
    return success(std::move(buildResult));
}

void BuildSystem::buildAsync(BuildCallback callback) {
    auto* scheduler = Services::getTaskScheduler();
    scheduler->schedule([this, cb = std::move(callback)]() {
        auto result = buildSync();
        if (result.hasValue()) {
            cb(result.value());
        } else {
            cb(BuildResult{false, "", result.error().message()});
        }
    });
}

} // namespace CodeWizard::BuildRun
