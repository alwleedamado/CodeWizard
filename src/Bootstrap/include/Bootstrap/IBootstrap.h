#pragma once

namespace CodeWizard::Bootstrap {

/**
 * @brief Controls the startup lifecycle of the IDE.
 */
class IBootstrap
{
public:
    virtual ~IBootstrap() = default;

    /**
     * @brief Initialize all modules and services.
     */
    virtual void initialize() = 0;

    /**
     * @brief Shutdown all modules gracefully.
     */
    virtual void shutdown() = 0;
};

} // namespace CodeWizard::Bootstrap
