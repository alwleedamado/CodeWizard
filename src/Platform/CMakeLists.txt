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
    internal/PlatformImpl.cpp
    internal/FileSystem.cpp
    internal/Threading.cpp
    internal/Timer.cpp
)

target_include_directories(PlatformImpl
    PRIVATE internal
)

# Implementation links to interface only
target_link_libraries(PlatformImpl
    PRIVATE Platform
)

# Optionally expose QtCore if needed in internal threading/timers
# target_link_libraries(PlatformImpl PRIVATE Qt6::Core)
