# Catch2 headless test for Platform
add_executable(PlatformTests
    PlatformTest.cpp
)

target_include_directories(PlatformTests
    PRIVATE
        ../..
        ../../src/Platform/include
)

# Link module implementation if needed
target_link_libraries(PlatformTests PRIVATE PlatformImpl)
