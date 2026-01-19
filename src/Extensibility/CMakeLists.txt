# src/Extensibility/CMakeLists.txt
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Extensibility STATIC ${SOURCES})

target_include_directories(Extensibility PUBLIC include)
target_link_libraries(Extensibility PUBLIC Core Platform)
target_compile_features(Extensibility PUBLIC cxx_std_20)

if(UNIX AND NOT APPLE)
    target_link_libraries(Extensibility PRIVATE pthread dl)
endif()
