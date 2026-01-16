# --------------------------
# Public interface
# --------------------------
add_library(Bootstrap INTERFACE)

target_link_libraries(Bootstrap
    INTERFACE
        Platform
        Services
        Extensibility
        Project
        Editor
        LanguageIntelligence
        BuildRun
        Runtime
        Persistence
        UI
)

target_include_directories(Bootstrap
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(BootstrapImpl STATIC
    internal/BootstrapImpl.cpp
)

target_include_directories(BootstrapImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)
