add_library(LanguageIntelligence INTERFACE)

target_include_directories(LanguageIntelligence
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# NOTE:
# This module exposes INTERFACES ONLY.
# Implementations must remain private and must not leak dependencies upward.
