add_library(BuildRun INTERFACE)

target_link_libraries(BuildRun
    INTERFACE Services
    INTERFACE Platform
)

target_include_directories(BuildRun
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(BuildRunImpl STATIC
    internal/BuildOrchestrator.cpp
    internal/ToolchainManager.cpp
    internal/Runner.cpp
    internal/BuildRunImpl.cpp
)
target_include_directories(BuildRunImpl PRIVATE internal)

target_link_libraries(BuildRunImpl
    PRIVATE Qt::Core
    PRIVATE BuildRun
    PRIVATE Services
    PRIVATE Platform
)
