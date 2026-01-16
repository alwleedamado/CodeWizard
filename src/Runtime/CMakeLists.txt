add_library(Runtime INTERFACE)
target_include_directories(Runtime INTERFACE include)

add_library(RuntimeImpl STATIC
    internal/DebuggerManager.cpp
    internal/DebugSession.cpp
    internal/ProcessController.cpp
)

target_include_directories(RuntimeImpl PRIVATE internal)
target_link_libraries(RuntimeImpl PRIVATE Runtime)

