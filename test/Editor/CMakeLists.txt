# Catch2 headless test for Editor
add_executable(EditorTests
    EditorTest.cpp
)

target_include_directories(EditorTests
    PRIVATE
        ../..
        ../../src/Editor/include
)

# Link module implementation if needed
target_link_libraries(EditorTests PRIVATE EditorImpl)
