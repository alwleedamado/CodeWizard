# tests/Platform/CMakeLists.txt
add_executable(PlatformTests
    PathTests.cpp
    FileSystemTests.cpp
)

target_link_libraries(PlatformTests PRIVATE Platform Catch2::Catch2WithMain)
target_compile_features(PlatformTests PRIVATE cxx_std_20)
add_test(NAME PlatformTests COMMAND PlatformTests)
