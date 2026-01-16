# Auto-generated CMakeLists.txt for BuildRun module

# --------------------------
# Public interface
# --------------------------
add_library(BuildRun INTERFACE)

target_include_directories(BuildRun
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(BuildRunImpl STATIC
    internal/BuildRunImpl.cpp
)

target_include_directories(BuildRunImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
