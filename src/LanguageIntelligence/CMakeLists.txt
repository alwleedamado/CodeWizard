# Auto-generated CMakeLists.txt for LanguageIntelligence module

# --------------------------
# Public interface
# --------------------------
add_library(LanguageIntelligence INTERFACE)

target_link_libraries(LanguageIntelligence
    INTERFACE Editor
    INTERFACE Services
)

target_include_directories(LanguageIntelligence
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(LanguageIntelligenceImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/LanguageIntelligenceImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/CompletionEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/DiagnosticsManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/LSPClient.cpp
)

target_include_directories(LanguageIntelligenceImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(LanguageIntelligenceImpl
    PRIVATE Qt::Core
    PRIVATE LanguageIntelligence
    PRIVATE Editor
    PRIVATE Services
)
