#pragma once

namespace CodeWizard::Editor {

/**
 * @brief Public interface for the Editor subsystem.
 *
 * Architectural constraints:
 * - No UI dependencies unless this is the UI module
 * - No concrete implementations exposed
 * - No cross-layer shortcuts
 */
class IEditor
{
public:
    virtual ~IEditor() = default;
};

} // namespace CodeWizard::Editor
