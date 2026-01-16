#pragma once

#include <string>

namespace CodeWizard::Editor::Internal {

/**
 * @brief Incremental parsing engine.
 *
 * Tracks text changes and updates syntax trees incrementally.
 * Designed for low-latency editing on large files.
 */
class IncrementalParser
{
public:
    void onTextInserted(size_t pos, size_t length);
    void onTextRemoved(size_t pos, size_t length);

    void reparseIfNeeded();
};

}
