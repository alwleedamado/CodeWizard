# Auto-generated CMakeLists.txt for Project module

# --------------------------
# Public interface
# --------------------------
add_library(Project INTERFACE)

target_include_directories(Project
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(ProjectImpl STATIC
    internal/ProjectImpl.cpp
)

target_include_directories(ProjectImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
