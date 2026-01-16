#include "FileSystem.h"
#include <filesystem>
namespace CodeWizard::Platform::Internal {
bool FileSystem::exists(const std::string& path) const {
    return std::filesystem::exists(path);
}
}
