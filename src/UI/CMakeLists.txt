# src/UI/CMakeLists.txt
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(UI STATIC ${SOURCES})

target_include_directories(UI PUBLIC include)
target_link_libraries(UI PUBLIC Core Platform Services Editor LanguageIntelligence Project)
target_compile_features(UI PUBLIC cxx_std_20)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(UI PUBLIC Qt6::Widgets)
