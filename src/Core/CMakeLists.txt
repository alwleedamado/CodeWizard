add_library(Core STATIC
    src/Logger.cpp
    src/Config.cpp
    src/EventBus.cpp
    src/ThreadPool.cpp
    src/UndoManager.cpp
)

target_include_directories(Core PUBLIC include)
target_link_libraries(Core PUBLIC Qt6::Widgets)