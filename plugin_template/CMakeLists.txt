# CMakeLists.txt for plugin
cmake_minimum_required(VERSION 3.16)
project(MyPlugin)

# Find CodeWizard core headers
find_path(CODEWIZARD_INCLUDE_DIR Plugin/Plugin.h)

add_library(MyPlugin SHARED
    MyPlugin.cpp
    plugin_exports.cpp
)

target_include_directories(MyPlugin PRIVATE ${CODEWIZARD_INCLUDE_DIR})
target_compile_definitions(MyPlugin PRIVATE PLUGIN_BUILD)

# Set output name
set_target_properties(MyPlugin PROPERTIES
    PREFIX ""  # No "lib" prefix on Linux
    OUTPUT_NAME "myplugin"
)

# Install to plugins directory
install(TARGETS MyPlugin
    LIBRARY DESTINATION plugins  # .so files
    RUNTIME DESTINATION plugins  # .dll files
)
