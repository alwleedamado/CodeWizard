add_library(LanguageIntelligence STATIC
    src/Diagnostic.cpp
    src/LspMessage.cpp
    src/LspClient.cpp
)


target_include_directories(LanguageIntelligence PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(LanguageIntelligence PUBLIC Core Platform Services Editor PRIVATE json)
target_compile_features(LanguageIntelligence PUBLIC cxx_std_20)

# Linux-specific
if(UNIX AND NOT APPLE)
    target_link_libraries(LanguageIntelligence PRIVATE pthread)
endif()
