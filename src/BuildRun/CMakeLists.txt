# --------------------------
# Public interface
# --------------------------
add_library(BuildRun INTERFACE)

target_link_libraries(BuildRun
    INTERFACE Services
    INTERFACE Platform
)

target_include_directories(BuildRun
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(BuildRunImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/BuildOrchestrator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/ToolchainManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/Runner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/BuildRunImpl.cpp
)
target_include_directories(BuildRunImpl PRIVATE internal)

target_link_libraries(BuildRunImpl
    PRIVATE Qt::Core
    PRIVATE BuildRun
    PRIVATE Services
    PRIVATE Platform
)
