# tests/Core/CMakeLists.txt
add_executable(CoreTests
    ResultTests.cpp
    TypesTests.cpp
    TaskTests.cpp
)

target_link_libraries(CoreTests PRIVATE Core Catch2::Catch2WithMain)
target_compile_features(CoreTests PRIVATE cxx_std_20)
add_test(NAME CoreTests COMMAND CoreTests)
