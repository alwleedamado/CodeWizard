// include/BuildRun/BuildSystem.h
#pragma once

#include <Core/Types.h>
#include <Core/Result.h>
#include <Platform/Path.h>
#include <functional>
#include <string>
using namespace CodeWizard::Core;
namespace CodeWizard::BuildRun {

struct BuildResult {
    bool success = false;
    std::string output;
    std::string error;
};

class BuildSystem {
public:
    using BuildCallback = std::function<void(const BuildResult&)>;

    explicit BuildSystem(const Platform::Path& workingDir);
    [[nodiscard]] Result<BuildResult> buildSync();
    void buildAsync(BuildCallback callback);

private:
    Platform::Path m_workingDir;
};

} // namespace CodeWizard::BuildRun
