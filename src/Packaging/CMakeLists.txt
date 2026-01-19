# src/Packaging/CMakeLists.txt
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Packaging STATIC ${SOURCES})

target_include_directories(Packaging PUBLIC include)
target_link_libraries(Packaging PUBLIC Core Platform)
target_compile_features(Packaging PUBLIC cxx_std_20)
