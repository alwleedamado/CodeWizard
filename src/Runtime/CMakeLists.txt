# src/Runtime/CMakeLists.txt
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Runtime STATIC ${SOURCES})

target_include_directories(Runtime PUBLIC include)
target_link_libraries(Runtime PUBLIC Core Services)
target_compile_features(Runtime PUBLIC cxx_std_20)
