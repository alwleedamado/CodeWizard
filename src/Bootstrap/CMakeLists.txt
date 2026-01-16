# Auto-generated CMakeLists.txt for Bootstrap module

# --------------------------
# Public interface
# --------------------------
add_library(Bootstrap INTERFACE)

target_include_directories(Bootstrap
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(BootstrapImpl STATIC
    internal/BootstrapImpl.cpp
)

target_include_directories(BootstrapImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
