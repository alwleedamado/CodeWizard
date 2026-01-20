file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(UI STATIC ${SOURCES})

target_include_directories(UI PUBLIC include)
target_link_libraries(UI PUBLIC Core Platform Services Editor LanguageIntelligence Project)
target_compile_features(UI PUBLIC cxx_std_20)

target_link_libraries(UI PRIVATE Qt6::Widgets)
