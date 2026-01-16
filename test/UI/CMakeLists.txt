# Catch2 headless test for UI
add_executable(UITests
    UITest.cpp
)

target_include_directories(UITests
    PRIVATE
        ../..
        ../../src/UI/include
)

# Link module implementation if needed
target_link_libraries(UITests PRIVATE UIImpl)
