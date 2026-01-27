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

# Find system Tree-sitter
find_package(PkgConfig REQUIRED)
pkg_check_modules(TREE_SITTER REQUIRED IMPORTED_TARGET tree-sitter)
# Function to handle Tree-sitter language linking
function(link_treesitter_language target lang)
    # Try pkg-config first
    pkg_check_modules(TREE_SITTER_${lang} QUIET tree-sitter-${lang})

    if(TREE_SITTER_${lang}_FOUND)
        target_link_libraries(${target} PRIVATE ${TREE_SITTER_${lang}_LIBRARIES})
        target_compile_definitions(${target} PRIVATE HAVE_TREE_SITTER_${lang})
        message(STATUS "Found tree-sitter-${lang} via pkg-config")
    else()
        # Fallback to direct library search
        find_library(TREE_SITTER_${lang}_LIB tree-sitter-${lang})
        if(TREE_SITTER_${lang}_LIB)
            target_link_libraries(${target} PRIVATE ${TREE_SITTER_${lang}_LIB})
            target_compile_definitions(${target} PRIVATE HAVE_TREE_SITTER_${lang})
            message(STATUS "Found tree-sitter-${lang} via direct search: ${TREE_SITTER_${lang}_LIB} HAVE_TREE_SITTER_${lang}")
        else()
            message(STATUS "tree-sitter-${lang} not found ")
        endif()
    endif()
endfunction()

# Performance-oriented defaults
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CodeWizard_ENABLE_HARDENING OFF CACHE BOOL "" FORCE)
    set(CodeWizard_ENABLE_SANITIZER_ADDRESS OFF CACHE BOOL "" FORCE)
    set(CodeWizard_ENABLE_SANITIZER_UNDEFINED OFF CACHE BOOL "" FORCE)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(cmake/PreventInSourceBuilds.cmake)
include(ProjectOptions.cmake)

CodeWizard_setup_options()

CodeWizard_global_options()
include(Dependencies.cmake)
CodeWizard_setup_dependencies()

CodeWizard_local_options()

target_compile_features(CodeWizard_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(CodeWizard::CodeWizard_options ALIAS CodeWizard_options)
add_library(CodeWizard::CodeWizard_warnings ALIAS CodeWizard_warnings)


# ---------------------------------------------------------------------------
# Output layout
# ---------------------------------------------------------------------------

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# ---------------------------------------------------------------------------
# Qt configuration (UI only – harmless)
# ---------------------------------------------------------------------------

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
)
# ---------------------------------------------------------------------------
# third party libs
# ---------------------------------------------------------------------------
add_subdirectory(libs)
# ---------------------------------------------------------------------------
# Subsystems
# Order reflects *dependency direction*, not build convenience
# ---------------------------------------------------------------------------
add_subdirectory(src/Core)
add_subdirectory(src/Platform)
add_subdirectory(src/Services)
add_subdirectory(src/Plugin)
add_subdirectory(src/Project)
add_subdirectory(src/Editor)
add_subdirectory(src/LanguageIntelligence)
add_subdirectory(src/BuildRun)
add_subdirectory(src/Runtime)
add_subdirectory(src/Packaging)
add_subdirectory(src/Persistence)
add_subdirectory(src/Theme)
add_subdirectory(src/UI)


# ---------------------------------------------------------------------------
# Final executable (composition root only)
# ---------------------------------------------------------------------------
# Add resource file
set(RESOURCES
        src/Editor/resources/queries/queries.qrc
)

qt6_add_resources(RESOURCE_SOURCES ${RESOURCES})

add_executable(CodeWizardApp
    src/main.cpp
    ${RESOURCE_SOURCES}
)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(-Oz -flto)
    add_link_options(-flto -s)

endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    target_compile_definitions(Editor PUBLIC
            $<$<CONFIG:Release>:QT_NO_DEBUG_OUTPUT>
    )
endif ()
target_link_libraries(CodeWizardApp
        PRIVATE tree-sitter
    PRIVATE
        # # global options
        CodeWizard_options
        # # warnings
        CodeWizard_warnings
        # modules
#        Core
#        Platform
#        Services
#        Extensibility
#        Project
#        Editor
#        LanguageIntelligence
#        BuildRun
        Runtime
#        Packaging
#        Persistence
#        UI
)
link_treesitter_language(Editor c)
link_treesitter_language(Editor cpp)
link_treesitter_language(Editor javascript)
link_treesitter_language(Editor typescript)
link_treesitter_language(Editor rust)
link_treesitter_language(Editor go)
link_treesitter_language(Editor python)


set_target_properties(CodeWizardApp PROPERTIES
    OUTPUT_NAME "CodeWizard"
)

include(CTest)

if(BUILD_TESTING)
    message(AUTHOR_WARNING "Building Tests. Be sure to check out test/constexpr_tests.cpp for constexpr testing")
    add_subdirectory(test)
endif()


if(CodeWizard_BUILD_FUZZ_TESTS)
    message(AUTHOR_WARNING "Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
    if (NOT CodeWizard_ENABLE_SANITIZER_ADDRESS AND NOT CodeWizard_ENABLE_SANITIZER_THREAD)
        message(WARNING "You need asan or tsan enabled for meaningful fuzz testing")
    endif()
    add_subdirectory(fuzz_test)

endif()

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if(MSVC)
    get_all_installable_targets(all_targets)
    message("all_targets=${all_targets}")
    set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT CodeWizardApp)

if(CMAKE_SKIP_INSTALL_RULES)
    return()
endif()

include(cmake/PackageProject.cmake)

# Add other targets that you want installed here, by default we just package the one executable
# we know we want to ship
CodeWizard_package_project(
  TARGETS
  CodeWizardApp
  CodeWizard_options
  CodeWizard_warnings
  # FIXME: this does not work! CK
  # PRIVATE_DEPENDENCIES_CONFIGURED project_options project_warnings
)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
