// include/Core/UndoRedo.h
#pragma once

#include <Core/Types.h>
#include <functional>
#include <vector>
#include <memory>

namespace CodeWizard::Core {

/// Generic undoable action interface
class UndoableAction {
public:
    virtual ~UndoableAction() = default;
    
    /// Perform the action (for redo or initial execution)
    virtual void execute() = 0;
    
    /// Revert the action (for undo)
    virtual void undo() = 0;
    
    /// Optional: name for debugging
    [[nodiscard]] virtual const char* name() const { return "Action"; }
};

/// Undo/Redo manager
class UndoRedoStack {
public:
    static constexpr size_t DEFAULT_MAX_STACK_SIZE = 1000;

    explicit UndoRedoStack(size_t maxStackSize = DEFAULT_MAX_STACK_SIZE)
        : m_maxStackSize(maxStackSize) {}

    /// Execute and record an action
    void execute(std::unique_ptr<UndoableAction> action) {
        action->execute();
        m_undoStack.push_back(std::move(action));
        
        // Trim stack if needed
        if (m_undoStack.size() > m_maxStackSize) {
            m_undoStack.erase(m_undoStack.begin());
        }
        
        // Clear redo stack
        m_redoStack.clear();
    }

    /// Undo the last action
    void undo() {
        if (m_undoStack.empty()) return;
        
        auto action = std::move(m_undoStack.back());
        m_undoStack.pop_back();
        
        action->undo();
        m_redoStack.push_back(std::move(action));
    }

    /// Redo the last undone action
    void redo() {
        if (m_redoStack.empty()) return;
        
        auto action = std::move(m_redoStack.back());
        m_redoStack.pop_back();
        
        action->execute();
        m_undoStack.push_back(std::move(action));
    }

    void clear() { m_undoStack.clear(); m_redoStack.clear(); }

    [[nodiscard]] bool canUndo() const noexcept { return !m_undoStack.empty(); }
    [[nodiscard]] bool canRedo() const noexcept { return !m_redoStack.empty(); }
    [[nodiscard]] size_t undoSize() const noexcept { return m_undoStack.size(); }
    [[nodiscard]] size_t redoSize() const noexcept { return m_redoStack.size(); }

private:
    std::vector<std::unique_ptr<UndoableAction>> m_undoStack;
    std::vector<std::unique_ptr<UndoableAction>> m_redoStack;
    size_t m_maxStackSize;
};

/// Helper macro to create simple undoable actions
#define CW_UNDOABLE_ACTION(class_name, exec_block, undo_block, action_text) \
    class class_name final : public CodeWizard::Core::UndoableAction { \
    public: \
        void execute() override { exec_block } \
        void undo() override { undo_block } \
        std::string name() const override { return action_text; } \
    };

} // namespace CodeWizard::Core