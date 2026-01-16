# --------------------------
# Public interface
# --------------------------
add_library(Extensibility INTERFACE)

target_link_libraries(Extensibility
    INTERFACE Services
    INTERFACE Platform
)

target_include_directories(Extensibility
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(ExtensibilityImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/ExtensibilityImpl.cpp
)

target_include_directories(ExtensibilityImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(ExtensibilityImpl
    PRIVATE Extensibility
    PRIVATE Services
    PRIVATE Platform
)
