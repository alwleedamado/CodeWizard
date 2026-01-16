#pragma once

#include <string>
#include <vector>

namespace CodeWizard::Project {

/**
 * @brief Represents a single project within the workspace.
 */
class IProject
{
public:
    virtual ~IProject() = default;

    /**
     * @brief Load the project from disk.
     */
    virtual bool load(const std::string& projectFile) = 0;

    /**
     * @brief Save the project to disk.
     */
    virtual bool save() = 0;

    /**
     * @brief List source files in the project.
     */
    virtual std::vector<std::string> sourceFiles() const = 0;
};

} // namespace CodeWizard::Project
