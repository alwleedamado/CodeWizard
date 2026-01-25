add_library(Core STATIC
    src/Logger.cpp
    src/Config.cpp
    src/EventBus.cpp
    src/ThreadPool.cpp
)

target_include_directories(Core PUBLIC include)
target_compile_features(Core PUBLIC cxx_std_20)
