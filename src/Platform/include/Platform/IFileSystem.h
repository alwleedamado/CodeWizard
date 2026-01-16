#pragma once
#include <string>
namespace CodeWizard::Platform {
class IFileSystem {
public:
    virtual ~IFileSystem() = default;
    virtual bool exists(const std::string& path) const = 0;
};
}
