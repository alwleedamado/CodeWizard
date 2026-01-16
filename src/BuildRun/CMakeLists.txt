add_library(BuildRun INTERFACE)
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
target_link_libraries(BuildRunImpl PRIVATE ProjectImpl PlatformImpl ServicesImpl Qt6::Core)
