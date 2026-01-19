# tests/Editor/CMakeLists.txt
add_executable(EditorTests
    TextBufferTests.cpp
    DocumentTests.cpp
)

target_link_libraries(EditorTests PRIVATE Editor Catch2::Catch2WithMain)
target_compile_features(EditorTests PRIVATE cxx_std_20)
add_test(NAME EditorTests COMMAND EditorTests)
