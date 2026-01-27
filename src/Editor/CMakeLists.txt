# src/Editor/CMakeLists.txt
qt6_wrap_cpp(UI_MOC_SRCS
    include/Editor/CodeTextEdit.h
    include/Editor/TreeSitterHighlighter.h
)


add_library(Editor STATIC
    ${UI_MOC_SRCS}
    src/CodeTextEdit.cpp
    src/TreeSitterHighlighter.cpp
    src/TreeSitterManager.cpp
)

target_include_directories(Editor PUBLIC include)
target_link_libraries(Editor
    PUBLIC Core Platform Services Theme Qt6::Core Qt6::Widgets
)