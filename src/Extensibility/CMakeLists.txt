# Auto-generated CMakeLists.txt for Extensibility module

# --------------------------
# Public interface
# --------------------------
add_library(Extensibility INTERFACE)

target_include_directories(Extensibility
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(ExtensibilityImpl STATIC
    internal/ExtensibilityImpl.cpp
)

target_include_directories(ExtensibilityImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
