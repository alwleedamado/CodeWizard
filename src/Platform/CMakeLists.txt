# Auto-generated CMakeLists.txt for Platform module

# --------------------------
# Public interface
# --------------------------
add_library(Platform INTERFACE)

target_include_directories(Platform
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(PlatformImpl STATIC
    internal/PlatformImpl.cpp
)

target_include_directories(PlatformImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
