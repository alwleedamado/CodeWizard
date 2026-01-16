#pragma once

#include <string>

namespace CodeWizard::Editor::Internal {

/**
 * @brief Syntax highlighting engine.
 *
 * Works on token streams provided by the incremental parser
 * or language intelligence.
 */
class SyntaxHighlighter
{
public:
    void updateHighlighting();
};

}
