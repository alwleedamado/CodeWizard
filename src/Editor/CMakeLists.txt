# src/Editor/CMakeLists.txt
add_library(Editor STATIC
    src/TextBuffer.cpp
    src/Document.cpp
    src/Cursor.cpp
)

target_include_directories(Editor PUBLIC include)
target_link_libraries(Editor PUBLIC Core Platform Services)
target_compile_features(Editor PUBLIC cxx_std_20)
