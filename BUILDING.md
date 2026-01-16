# Building CodeWizard

## 1. Supported Platforms

CodeWizard currently targets:

* Linux (primary development and supported platform)

Planned support:

* Windows
* macOS

The build system is designed to be portable, but Linux is the reference platform.

---

## 2. Toolchain Requirements

### Required

* CMake 3.21 or newer
* A C++20–compatible compiler:

  * GCC 11 or newer
  * Clang 14 or newer
* Qt 6 (Core and Widgets modules)

### Strongly Recommended

* Ninja (faster builds)
* ccache or sccache (compiler caching)
* clang-tidy (static analysis)
* cppcheck (static analysis)
* Doxygen (documentation generation)

---

## 3. Dependency Management

CodeWizard uses CPM.cmake for dependency management.

Key properties:

* Reproducible builds through version pinning
* No vendored third-party source trees
* Clear visibility of all external dependencies

Examples of managed dependencies:

* Catch2 (testing framework)
* Additional libraries may be added via CPM as needed

---

## 4. Configuring and Building

### Standard Build

From the project root:

cmake -S . -B build -G Ninja
cmake --build build

### Release Build

cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release

---

## 5. Compiler Warnings and Hardening

The build system enables a sensible default set of compiler warnings and hardening flags.

Enabled by default:

* Strict but non-noisy compiler warnings
* Stack protection
* FORTIFY_SOURCE (where supported)
* Control-flow protection (where available)

Warnings can be promoted to errors using a configuration option:

-DCODEWIZARD_WARNINGS_AS_ERRORS=ON

---

## 6. Sanitizers

The following sanitizers are supported:

* AddressSanitizer (ASan)
* UndefinedBehaviorSanitizer (UBSan)
* ThreadSanitizer (TSan)

Sanitizers are intended for Debug or RelWithDebInfo builds and are automatically disabled for Release builds.

Example:

-DCODEWIZARD_ENABLE_ASAN=ON

---

## 7. Static Analysis

Static analysis is integrated into the build system.

Supported tools:

* clang-tidy
* cppcheck

These tools can be enabled or disabled via CMake options and are suitable for both local development and CI environments.

---

## 8. Testing

### Unit Testing

* Framework: Catch2 (v3+)
* Tests are built as separate targets
* Tests link against implementation libraries only, never UI components

To run tests:

ctest --test-dir build

---

### Fuzz Testing

Fuzz targets are supported for critical subsystems such as:

* Protocol parsing
* Language server communication
* Input handling in core logic

Fuzzing is typically combined with sanitizers.

---

## 9. Compiler Caching

Compiler caching is supported and automatically detected if available.

Supported tools:

* ccache
* sccache

Caching significantly reduces incremental build times.

---

## 10. Interprocedural Optimization (IPO / LTO)

Interprocedural optimization is supported and can be enabled explicitly.

Example:

-DCODEWIZARD_ENABLE_IPO=ON

IPO/LTO is intended for Release builds and may increase build times.

---

## 11. Documentation

API documentation is generated using Doxygen.

Public interfaces are expected to be documented.

A documentation target is provided by the build system.

---

## 12. Packaging

Packaging is handled via CPack.

Supported output formats include:

* TGZ
* ZIP
* DEB (Linux)

Packaging produces redistributable artifacts suitable for end users.

---

## 13. Build Verification

A successful build is expected to:

* Compile all targets without warnings (or with explicitly accepted warnings)
* Launch the Qt-based GUI
* Run unit tests successfully
* Report no sanitizer or static analysis errors (when enabled)

---

## 14. Build Philosophy

The build system is designed to:

* Fail fast on configuration or compilation errors
* Surface problems early during development
* Enforce architectural and quality constraints
* Scale with project size and complexity

Build correctness, reproducibility, and transparency are non-negotiable.
