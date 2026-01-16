#pragma once

namespace CodeWizard::Extensibility {

/**
 * @brief Public interface for the Extensibility subsystem.
 *
 * Architectural constraints:
 * - No UI dependencies unless this is the UI module
 * - No concrete implementations exposed
 * - No cross-layer shortcuts
 */
class IExtensibility
{
public:
    virtual ~IExtensibility() = default;
};

} // namespace CodeWizard::Extensibility
