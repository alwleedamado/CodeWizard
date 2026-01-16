cmake_minimum_required(VERSION 3.21)

project(CodeWizard
    VERSION 0.1.0
    DESCRIPTION "CodeWizard IDE"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Global compiler configuration
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Performance-oriented defaults
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

option(CODEWIZARD_ENABLE_RTTI "Enable RTTI" ON)
option(CODEWIZARD_ENABLE_EXCEPTIONS "Enable exceptions" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ---------------------------------------------------------------------------
# Output layout
# ---------------------------------------------------------------------------

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---------------------------------------------------------------------------
# Testing (Catch2)
# ---------------------------------------------------------------------------

include(FetchContent)

option(CODEWIZARD_BUILD_TESTS "Build CodeWizard tests" ON)

if (CODEWIZARD_BUILD_TESTS)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)
endif()

# ---------------------------------------------------------------------------
# Qt configuration (UI only – harmless)
# ---------------------------------------------------------------------------

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
)

# ---------------------------------------------------------------------------
# Global include policy
# ---------------------------------------------------------------------------

# Only INTERFACE includes are allowed across module boundaries
set(CODEWIZARD_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# ---------------------------------------------------------------------------
# Subsystems
# Order reflects *dependency direction*, not build convenience
# ---------------------------------------------------------------------------

add_subdirectory(src/Platform)
add_subdirectory(src/Services)
add_subdirectory(src/Extensibility)

add_subdirectory(src/Project)
add_subdirectory(src/Editor)
add_subdirectory(src/LanguageIntelligence)
add_subdirectory(src/BuildRun)
add_subdirectory(src/Runtime)
add_subdirectory(src/Persistence)

add_subdirectory(src/UI)
add_subdirectory(src/Bootstrap)

# Packaging is meta / optional
# add_subdirectory(src/Packaging)

# ---------------------------------------------------------------------------
# Final executable (composition root only)
# ---------------------------------------------------------------------------

add_executable(CodeWizardApp
    src/main.cpp
)

target_link_libraries(CodeWizardApp
    PRIVATE
        Qt6::Core
        Qt6::Widgets

        Platform
        Services
        Extensibility
        Project
        Editor
        LanguageIntelligence
        BuildRun
        Runtime
        Persistence
        UI
        Bootstrap
)

set_target_properties(CodeWizardApp PROPERTIES
    OUTPUT_NAME "CodeWizard"
)
