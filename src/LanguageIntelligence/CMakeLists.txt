# src/LanguageIntelligence/CMakeLists.txt
add_library(LanguageIntelligence STATIC
    src/Diagnostic.cpp
    src/LspMessage.cpp
    src/LspClient.cpp
)


target_link_libraries(LanguageIntelligence PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(LanguageIntelligence PUBLIC include)
target_link_libraries(LanguageIntelligence PUBLIC Core Platform Services Editor)
target_compile_features(LanguageIntelligence PUBLIC cxx_std_20)

# Linux-specific
if(UNIX AND NOT APPLE)
    target_link_libraries(LanguageIntelligence PRIVATE pthread)
endif()
