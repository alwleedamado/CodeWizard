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
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/EditorImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/EditorCore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/EditorManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/IncrementalParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/SyntaxHighlighter.cpp
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
    PRIVATE LanguageIntelligence
)
