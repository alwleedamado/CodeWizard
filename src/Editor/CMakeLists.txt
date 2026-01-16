# --------------------------
# Public interface
# --------------------------
add_library(Editor INTERFACE)

target_link_libraries(Editor
    INTERFACE Services
    INTERFACE Platform
    INTERFACE Extensibility
)

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

target_link_libraries(EditorImpl
    PRIVATE Qt::Core
    PRIVATE Editor
    PRIVATE Services
    PRIVATE Platform
    PRIVATE Extensibility
)
