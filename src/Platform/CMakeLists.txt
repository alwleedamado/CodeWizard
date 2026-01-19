# src/Platform/CMakeLists.txt
add_library(Platform STATIC
    src/Path.cpp
    src/FileSystem.cpp
    src/Timer.cpp
)

target_include_directories(Platform PUBLIC include)
target_link_libraries(Platform PUBLIC Core)
target_compile_features(Platform PUBLIC cxx_std_20)

# Link filesystem (required for std::filesystem)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_link_libraries(Platform PRIVATE stdc++fs)
endif()
