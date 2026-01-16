# ---------------------------------------------------------------------------
# Services module
# ---------------------------------------------------------------------------

# Public headers
add_library(Services INTERFACE)
target_include_directories(Services
    INTERFACE include
)

# Services depend on Platform
target_link_libraries(Services
    INTERFACE Platform
)

# Internal implementation
add_library(ServicesImpl STATIC
    internal/ServiceImpl.cpp
    internal/ServiceRegistry.cpp
    internal/EventBus.cpp
    internal/TaskScheduler.cpp
)

target_include_directories(ServicesImpl
    PRIVATE internal
)

# Implementation links
target_link_libraries(ServicesImpl
    PRIVATE Services
    PRIVATE Platform
)
