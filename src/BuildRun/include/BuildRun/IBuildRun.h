#pragma once
#include <string>
#include <vector>

namespace CodeWizard::BuildRun {

struct BuildTarget
{
    std::string name;
    std::string type;
    std::vector<std::string> sources;
};

class IBuildRun
{
public:
    virtual ~IBuildRun() = default;
    virtual bool build(const BuildTarget& target) = 0;
    virtual bool run(const std::string& targetName, const std::vector<std::string>& args) = 0;
    virtual void setToolchain(const std::string& name) = 0;
};

} // namespace CodeWizard::BuildRun
