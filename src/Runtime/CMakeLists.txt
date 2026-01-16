add_library(Runtime INTERFACE)

target_link_libraries(Runtime
    INTERFACE Services
    INTERFACE BuildRun
)

target_include_directories(Runtime INTERFACE include)

add_library(RuntimeImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/DebuggerManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/DebugSession.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/ProcessController.cpp
)

target_include_directories(RuntimeImpl PRIVATE internal)

target_link_libraries(RuntimeImpl
    PRIVATE Runtime
    PRIVATE Services
    PRIVATE BuildRun
)
