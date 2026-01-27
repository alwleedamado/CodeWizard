#include "Core/UndoManager.h"
#include <QApplication>

using namespace CodeWizard::Core;

UndoManager& UndoManager::instance() {
  static UndoManager inst;
  return inst;
}

UndoManager::UndoManager() {
  m_editorGroup = std::make_unique<QUndoGroup>(qApp);
}

UndoManager::Handle UndoManager::registerStack(QUndoStack* stack, const QString& groupName) {
  if (!stack) return Handle{};

  QUndoGroup* targetGroup;
  if (groupName == "editors") {
    targetGroup = m_editorGroup.get();
  } else {
    targetGroup = getGroup(groupName);
  }

  targetGroup->addStack(stack);
  return Handle{targetGroup, stack};
}

QUndoGroup* UndoManager::getGroup(const QString& name) {
  // Find existing group
  for (const auto& [groupName, group] : m_groupMap) {
    if (groupName == name) {
      return group;
    }
  }

  // Create new group
  auto newGroup = std::make_unique<QUndoGroup>(qApp);
  QUndoGroup* rawPtr = newGroup.get();
  m_groups.push_back(std::move(newGroup));
  m_groupMap.emplace_back(name, rawPtr);

  return rawPtr;
}

// Handle implementation
UndoManager::Handle::Handle(QUndoGroup* group, QUndoStack* stack)
    : m_group(group), m_stack(stack) {}

UndoManager::Handle::Handle(Handle&& other) noexcept
    : m_group(other.m_group), m_stack(other.m_stack) {
  other.m_group = nullptr;
  other.m_stack = nullptr;
}

UndoManager::Handle& UndoManager::Handle::operator=(Handle&& other) noexcept {
  if (this != &other) {
    // Clean up current
    if (m_group && m_stack) {
      m_group->removeStack(m_stack);
    }

    m_group = other.m_group;
    m_stack = other.m_stack;
    other.m_group = nullptr;
    other.m_stack = nullptr;
  }
  return *this;
}

UndoManager::Handle::~Handle() {
  if (m_group && m_stack) {
    m_group->removeStack(m_stack);
  }
}