#pragma once

#include <string>

namespace CodeWizard::Runtime::Internal {

/**
 * @brief Controls launching and stopping runtime processes.
 *
 * Abstracts OS-level process handling.
 */
class ProcessController
{
public:
    bool launch(const std::string& executablePath);
    void terminate();

private:
    int m_pid{-1};
};

} // namespace CodeWizard::Runtime::Internal

