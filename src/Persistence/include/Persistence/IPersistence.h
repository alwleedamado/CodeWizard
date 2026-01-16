#pragma once

namespace CodeWizard::Persistence {

/**
 * @brief Public interface for the Persistence subsystem.
 *
 * Architectural constraints:
 * - No UI dependencies unless this is the UI module
 * - No concrete implementations exposed
 * - No cross-layer shortcuts
 */
class IPersistence
{
public:
    virtual ~IPersistence() = default;
};

} // namespace CodeWizard::Persistence
