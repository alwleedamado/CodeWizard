# src/Project/CMakeLists.txt
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Project STATIC ${SOURCES})

target_include_directories(Project PUBLIC include)
target_link_libraries(Project PUBLIC Core Platform)
target_compile_features(Project PUBLIC cxx_std_20)
