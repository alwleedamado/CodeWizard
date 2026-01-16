#pragma once

namespace CodeWizard::UI {

/**
 * @brief Public interface for the UI subsystem.
 *
 * Architectural constraints:
 * - No UI dependencies unless this is the UI module
 * - No concrete implementations exposed
 * - No cross-layer shortcuts
 */
class IUI
{
public:
    virtual ~IUI() = default;
};

} // namespace CodeWizard::UI
