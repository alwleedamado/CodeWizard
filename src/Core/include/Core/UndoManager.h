#pragma once

#include <QUndoGroup>
#include <QUndoStack>
#include <memory>
#include <vector>
#include <QString>

namespace CodeWizard::Core {

class UndoManager {
public:
  static UndoManager& instance();

  // Editor-specific group (shared by all editors)
  [[nodiscard]] QUndoGroup* editorGroup() const { return m_editorGroup.get(); }

  // Register any undo stack with optional group name
  class Handle {
  public:
    Handle() = default;
    Handle(Handle&&) noexcept;
    Handle& operator=(Handle&&) noexcept;
    ~Handle();

    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

  private:
    friend class UndoManager;
    Handle(QUndoGroup* group, QUndoStack* stack);
    QUndoGroup* m_group = nullptr;
    QUndoStack* m_stack = nullptr;
  };

  [[nodiscard]] Handle registerStack(QUndoStack* stack, const QString& groupName = "default");

  // Get/create named group
  [[nodiscard]] QUndoGroup* getGroup(const QString& name);

private:
  UndoManager();
  ~UndoManager() = default;

  std::unique_ptr<QUndoGroup> m_editorGroup;
  std::vector<std::unique_ptr<QUndoGroup>> m_groups;
  std::vector<std::pair<QString, QUndoGroup*>> m_groupMap;
};

} // namespace CodeWizard::Core