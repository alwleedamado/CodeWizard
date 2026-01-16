# CodeWizard Architecture

## 1. Purpose

CodeWizard is a lightweight, modular, extensible IDE designed with the following principles:

* Strict separation of concerns
* Explicit dependency direction
* Long-term extensibility without architectural rewrites
* High performance on low-end hardware
* Legal and architectural independence from existing IDEs

This document defines the architectural contract of the project. Any change that violates this contract must be treated as a breaking architectural change.

---

## 2. High-Level Design

CodeWizard follows a layered, service-oriented architecture with a single, explicit composition root.

High-level layering:

Bootstrap (composition root)
→ UI (Qt-only)
→ Feature modules (Editor, Project, Language Intelligence, Build/Run, Runtime)
→ Services
→ Platform

The Platform layer is at the bottom and has no dependencies on higher layers. All dependencies flow downward.

---

## 3. Core Architectural Rules

### 3.1 Dependency Direction (Strict)

Dependencies are one-directional and enforced at the build system level.

Allowed dependency direction:

UI → Feature Modules → Services → Platform

Forbidden:

* Platform depending on any higher layer
* Feature modules depending on UI
* Cyclic dependencies between modules

Any violation is considered an architectural error.

---

### 3.2 Interface / Implementation Separation

Each module follows a strict public/private split:

* Public interfaces live in `include/<Module>/`
* Private implementations live in `internal/`

Rules:

* Only headers in `include/` may be included across module boundaries
* Headers in `internal/` must never be included by other modules
* Each module exposes an INTERFACE library and links its implementation separately

This ensures clear contracts, fast compilation, and long-term stability.

---

### 3.3 Qt Isolation Rule

Qt is strictly confined to the UI module.

Rules:

* No Qt headers outside `src/UI`
* No QObject inheritance outside UI
* No Qt types in public interfaces

This guarantees:

* Headless testing of all non-UI modules
* UI replacement capability in the future
* Reduced compile times and coupling

---

## 4. Module Responsibilities

### Bootstrap

* Application entry point
* Composition root
* Wires together all modules and services
* Owns application lifetime

Bootstrap contains no business logic.

---

### Platform

Low-level OS and runtime abstractions:

* Threading and synchronization
* Timers and clocks
* Filesystem and environment access
* Process and system interaction

Platform is deliberately minimal and dependency-free.

---

### Services

Shared infrastructure services:

* Event bus
* Task scheduler
* Service registry / locator
* Logging and diagnostics

Services are designed to be lightweight, testable, and reusable.

---

### Editor

Editor core, independent of UI:

* Documents
* Text buffers
* Undo/redo infrastructure
* Editor state and models

Editor logic must never depend on UI or Qt.

---

### LanguageIntelligence

Language-aware services:

* Language Server Protocol (LSP) client
* clangd integration
* Diagnostics, symbols, and completion
* Indexing and background analysis

Designed to be asynchronous and resilient to failure.

---

### Project

Project and workspace management:

* Workspace model
* File trees
* Build targets
* Project metadata

---

### BuildRun

Build orchestration and toolchain integration:

* Build system abstraction
* Compiler and toolchain invocation
* Build output collection

---

### Runtime

Execution and debugging support:

* Process execution
* Debug session management
* Runtime monitoring

---

### Persistence

State persistence and configuration:

* Settings storage
* Session state
* Layout and UI state persistence

---

### Extensibility

Plugin infrastructure:

* Plugin interfaces and contracts
* Plugin discovery and lifecycle
* Stable ABI boundaries

Plugins interact with the IDE only through defined interfaces.

---

### UI

User interface layer (Qt):

* Qt Widgets and windows
* Visual composition and layout
* Input handling
* Presentation logic

UI consumes services and domain modules but contains no core logic.

---

## 5. Plugin Architecture

Plugins are optional, isolated components that extend the IDE.

Rules:

* Plugins depend only on public interfaces
* Plugins must not include or link against internal implementations
* Plugin interfaces are versioned and ABI-stable across minor releases

Plugin loading and failure must never destabilize the core application.

---

## 6. Performance Principles

Performance is treated as a first-class feature.

Guidelines:

* Avoid heap allocations in hot paths
* Prefer asynchronous and non-blocking operations
* Perform indexing and analysis in background threads
* Avoid RTTI and exceptions outside the UI where possible
* Support IPO/LTO for release builds

---

## 7. Legal and Design Independence

The architecture of CodeWizard is independently designed and based on general, industry-standard software engineering principles.

It is not derived from, copied from, or structurally dependent on any existing IDE (including Qt Creator or others).

---

## 8. Architectural Stability

This architecture is intended to remain stable for the lifetime of the project.

New functionality should be added by:

* Introducing new modules
* Extending existing interfaces
* Adding plugins

Rewriting or collapsing layers is strongly discouraged.
