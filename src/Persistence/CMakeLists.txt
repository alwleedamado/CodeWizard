# src/Persistence/CMakeLists.txt
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Persistence STATIC ${SOURCES})

target_include_directories(Persistence PUBLIC include)
target_link_libraries(Persistence PUBLIC Core Platform)
target_compile_features(Persistence PUBLIC cxx_std_20)

target_link_libraries(Persistence PUBLIC nlohmann_json::nlohmann_json)

