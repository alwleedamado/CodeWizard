#pragma once

namespace CodeWizard::Runtime {

/**
 * @brief Represents a live debug session.
 */
class IDebugSession
{
public:
    virtual ~IDebugSession() = default;

    virtual void start() = 0;
    virtual void terminate() = 0;

    virtual bool isRunning() const = 0;
};

} // namespace CodeWizard::Runtime

