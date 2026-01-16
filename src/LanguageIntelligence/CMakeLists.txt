# Auto-generated CMakeLists.txt for LanguageIntelligence module

# --------------------------
# Public interface
# --------------------------
add_library(LanguageIntelligence INTERFACE)

target_include_directories(LanguageIntelligence
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(LanguageIntelligenceImpl STATIC
    internal/LanguageIntelligenceImpl.cpp
)

target_include_directories(LanguageIntelligenceImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
target_link_libraries(LanguageIntelligenceImpl PRIVATE Qt6::Core)
