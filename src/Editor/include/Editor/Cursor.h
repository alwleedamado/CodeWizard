// include/Editor/Cursor.h
#pragma once

#include <Core/Types.h>
#include <vector>

namespace CodeWizard::Editor {

class Cursor {
public:
    explicit Cursor(Core::Position pos = {}) : m_position(pos) {}

    [[nodiscard]] Core::Position position() const noexcept { return m_position; }
    void setPosition(Core::Position pos) { m_position = pos; }

    // Selection support
    [[nodiscard]] bool hasSelection() const noexcept { return m_anchor != m_position; }
    [[nodiscard]] Core::Position anchor() const noexcept { return m_anchor; }
    void setAnchor(Core::Position anchor) { m_anchor = anchor; }

    [[nodiscard]] Core::TextRange selection() const;

private:
    Core::Position m_position;
    Core::Position m_anchor = m_position;
};

} // namespace CodeWizard::Editor
