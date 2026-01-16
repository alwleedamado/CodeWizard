# Catch2 headless test for BuildRun
add_executable(BuildRunTests
    BuildRunTest.cpp
)

target_include_directories(BuildRunTests
    PRIVATE
        ../..
        ../../src/BuildRun/include
)

# Link module implementation if needed
target_link_libraries(BuildRunTests PRIVATE BuildRunImpl)
