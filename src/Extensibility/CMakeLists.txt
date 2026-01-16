add_library(Extensibility INTERFACE)

target_include_directories(Extensibility
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# NOTE:
# This module exposes INTERFACES ONLY.
# Implementations must remain private and must not leak dependencies upward.
