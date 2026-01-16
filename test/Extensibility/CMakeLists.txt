# Catch2 headless test for Extensibility
add_executable(ExtensibilityTests
    ExtensibilityTest.cpp
)

target_include_directories(ExtensibilityTests
    PRIVATE
        ../..
        ../../src/Extensibility/include
)

# Link module implementation if needed
target_link_libraries(ExtensibilityTests PRIVATE ExtensibilityImpl)
