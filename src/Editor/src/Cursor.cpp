// src/Cursor.cpp
#include "Editor/Cursor.h"

namespace CodeWizard::Editor {

Core::TextRange Cursor::selection() const {
    if (!hasSelection()) {
        return {m_position, m_position};
    }
    return (m_anchor < m_position) ? Core::TextRange{m_anchor, m_position} : Core::TextRange{m_position, m_anchor};
}

} // namespace CodeWizard::Editor
