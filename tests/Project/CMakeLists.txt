# Catch2 headless test for Project
add_executable(ProjectTests
    ProjectTest.cpp
)

target_include_directories(ProjectTests
    PRIVATE
        ../..
        ../../src/Project/include
)

# Link module implementation if needed
target_link_libraries(ProjectTests PRIVATE ProjectImpl)
