# Auto-generated CMakeLists.txt for Editor module

# --------------------------
# Public interface
# --------------------------
add_library(Editor INTERFACE)

target_include_directories(Editor
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(EditorImpl STATIC
    internal/EditorImpl.cpp
)

target_include_directories(EditorImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
target_link_libraries(EditorImpl PRIVATE Qt6::Core)
