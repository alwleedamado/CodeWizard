# src/BuildRun/CMakeLists.txt
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(BuildRun STATIC ${SOURCES})

target_include_directories(BuildRun PUBLIC include)
target_link_libraries(BuildRun PUBLIC Core Platform Services)
target_compile_features(BuildRun PUBLIC cxx_std_20)

if(UNIX AND NOT APPLE)
    target_link_libraries(BuildRun PRIVATE pthread dl)
endif()
