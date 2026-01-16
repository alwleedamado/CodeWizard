#pragma once
#include "Platform/IFileSystem.h"
namespace CodeWizard::Platform::Internal {
class FileSystem final : public IFileSystem {
public:
    bool exists(const std::string& path) const override;
};
}
