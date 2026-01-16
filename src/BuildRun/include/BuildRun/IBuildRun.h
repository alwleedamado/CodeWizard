#pragma once

namespace CodeWizard::BuildRun {

/**
 * @brief Public interface for the BuildRun subsystem.
 *
 * Architectural constraints:
 * - No UI dependencies unless this is the UI module
 * - No concrete implementations exposed
 * - No cross-layer shortcuts
 */
class IBuildRun
{
public:
    virtual ~IBuildRun() = default;
};

} // namespace CodeWizard::BuildRun
