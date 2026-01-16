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

# Fetch Catch2 (v3)
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0  # Replace with desired version
)
FetchContent_MakeAvailable(catch2)

# Performance-oriented defaults
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# Avoid unnecessary RTTI/exceptions outside UI if desired later
option(CODEWIZARD_ENABLE_RTTI "Enable RTTI" ON)
option(CODEWIZARD_ENABLE_EXCEPTIONS "Enable exceptions" ON)

# Folder grouping in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ---------------------------------------------------------------------------
# Qt configuration (kept minimal and localized)
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
# Subsystems (strict order reflects dependency direction)
# ---------------------------------------------------------------------------

add_subdirectory(src/Bootstrap)
add_subdirectory(src/Platform)
add_subdirectory(src/Services)
add_subdirectory(src/Project)
add_subdirectory(src/Editor)
add_subdirectory(src/LanguageIntelligence)
add_subdirectory(src/BuildRun)
add_subdirectory(src/Persistence)
add_subdirectory(src/Extensibility)
add_subdirectory(src/UI)

# ---------------------------------------------------------------------------
# Final executable
# ---------------------------------------------------------------------------

add_executable(codewizard
    src/main.cpp
)

target_link_libraries(codewizard
    PRIVATE
        Qt6::Core
        Qt6::Widgets

        Bootstrap
        Platform
        Services
        Project
        Editor
        LanguageIntelligence
        BuildRun
        Persistence
        Extensibility
        UI
)

set_target_properties(codewizard PROPERTIES
    OUTPUT_NAME "CodeWizard"
)
