// src/Document.cpp
#include "Editor/Document.h"

#include "../../../build/Desktop-Debug/_deps/catch2-src/src/catch2/internal/catch_stdstreams.hpp"

#include <Platform/FileSystem.h>
#include <Services/GlobalServices.h>
#include <algorithm>

namespace CodeWizard::Editor {

Document::Document() = default;

Document::Document(const Platform::Path& filePath) : m_filePath(filePath) {}

Result<void> Document::loadFromFile() {
    if (m_filePath.native().empty()) {
        return failure(Core::ErrorCode::InvalidArgument, "No file path set");
    }

    auto result = Platform::readFile(m_filePath);
    if (!result.hasValue()) {
        return failure(result.error().code(), result.error().message());
    }

    m_buffer = TextBuffer(result.value());
    m_isModified = false;
    m_undoStack.clear();
    m_redoStack.clear();
    return success();
}

Result<void> Document::saveToFile() {
    if (m_filePath.native().empty()) {
        return failure(Core::ErrorCode::InvalidArgument, "No file path set");
    }

    auto result = Platform::writeFile(m_filePath, m_buffer.text());
    if (!result.hasValue()) {
        return failure(result.error().code(), result.error().message());
    }

    m_isModified = false;
    return success();
}

Result<void> Document::saveAs(const Platform::Path& newPath) {
    auto result = Platform::writeFile(newPath, m_buffer.text());
    if (!result.hasValue()) {
        return failure(result.error().code(), result.error().message());
    }

    m_filePath = newPath;
    m_isModified = false;
    return success();
}

void Document::insertText(Core::Position pos, std::string_view text) {
    Change change;
    change.range = {pos, pos};
    change.newText = std::string(text);
    applyChange(change);
}

void Document::removeText(Core::TextRange range) {
    if (!range.valid()) return;
    std::string oldText(m_buffer.text().substr(
        m_buffer.offsetFromPosition(range.start),
        m_buffer.offsetFromPosition(range.end) - m_buffer.offsetFromPosition(range.start)
    ));
    Change change;
    change.range = range;
    change.oldText = std::move(oldText);
    applyChange(change);
}

void Document::applyChange(const Change& change, bool isUndo) {
    if (!isUndo) {
        // Record for undo
        if (m_undoStack.size() >= MAX_UNDO_STACK_SIZE) {
            m_undoStack.erase(m_undoStack.begin());
        }
        m_undoStack.push_back(change);
        m_redoStack.clear();
    }

    if (!change.oldText.empty()) {
        // This is a removal or replacement
        m_buffer.removeText(change.range);
        if (!change.newText.empty()) {
            m_buffer.insertText(change.range.start, change.newText);
        }
    } else {
        // This is an insertion
        m_buffer.insertText(change.range.start, change.newText);
    }

    markModified();
    if (m_changeCallback) {
        m_changeCallback(change);
    }
}

void Document::markModified() {
    m_isModified = true;
}

void Document::undo() {
    if (m_undoStack.empty()) return;
    Change change = std::move(m_undoStack.back());
    m_undoStack.pop_back();
    Catch::cout() << change.oldText;
    // Reverse the change
    Change reverse;
    reverse.range = change.range;
    reverse.oldText = change.newText;
    reverse.newText = change.oldText;
    applyChange(reverse, true);

    m_redoStack.push_back(change);
}

void Document::redo() {
    if (m_redoStack.empty()) return;
    Change change = std::move(m_redoStack.back());
    m_redoStack.pop_back();
    applyChange(change, true);
    m_undoStack.push_back(change);
}

bool Document::canUndo() const noexcept {
    return !m_undoStack.empty();
}

bool Document::canRedo() const noexcept {
    return !m_redoStack.empty();
}

void Document::setChangeCallback(ChangeCallback callback) {
    m_changeCallback = std::move(callback);
}

} // namespace CodeWizard::Editor
