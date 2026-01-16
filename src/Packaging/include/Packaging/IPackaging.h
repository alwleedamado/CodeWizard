#pragma once
#include <string>

namespace CodeWizard::Packaging {

class IPackaging
{
public:
    virtual ~IPackaging() = default;
    virtual bool createReleasePackage(const std::string& projectName, const std::string& outputPath) = 0;
    virtual bool createDebugPackage(const std::string& projectName, const std::string& outputPath) = 0;
};

} // namespace CodeWizard::Packaging
