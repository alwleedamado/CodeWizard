# Catch2 headless test for LanguageIntelligence
add_executable(LanguageIntelligenceTests
    LanguageIntelligenceTest.cpp
)

target_include_directories(LanguageIntelligenceTests
    PRIVATE
        ../..
        ../../src/LanguageIntelligence/include
)

# Link module implementation if needed
target_link_libraries(LanguageIntelligenceTests PRIVATE LanguageIntelligenceImpl)
