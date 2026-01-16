# ---------------------------------------------------------------------------
# Platform module
# ---------------------------------------------------------------------------

# Public headers
add_library(Platform INTERFACE)
target_include_directories(Platform
    INTERFACE include
)

# Internal implementation
add_library(PlatformImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/PlatformImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/FileSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/Threading.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/Timer.cpp
)

target_include_directories(PlatformImpl
    PRIVATE internal
)

# Implementation links to interface only
target_link_libraries(PlatformImpl
    PRIVATE Platform
)
