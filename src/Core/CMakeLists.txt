# src/Core/CMakeLists.txt
add_library(Core STATIC
    src/Logger.cpp
    src/Config.cpp
    src/Task.cpp
)

target_include_directories(Core PUBLIC include)
target_compile_features(Core PUBLIC cxx_std_20)

# No dependencies
