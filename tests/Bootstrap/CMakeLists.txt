# Catch2 headless test for Bootstrap
add_executable(BootstrapTests
    BootstrapTest.cpp
)

target_include_directories(BootstrapTests
    PRIVATE
        ../..
        ../../src/Bootstrap/include
)

# Link module implementation if needed
target_link_libraries(BootstrapTests PRIVATE BootstrapImpl)
