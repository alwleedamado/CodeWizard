find_package(Qt6 REQUIRED COMPONENTS Core)

# Plugin module
file(GLOB_RECURSE PLUGIN_SOURCES "src/*.cpp")
qt6_wrap_cpp(PLUGIN_MOC_SRCS include/Plugin/PluginManager.h)

add_library(Plugin STATIC ${PLUGIN_SOURCES} ${PLUGIN_MOC_SRCS})

target_include_directories(Plugin PUBLIC include)
target_link_libraries(Plugin PUBLIC Qt6::Core)
target_compile_features(Plugin PUBLIC cxx_std_20)

# Install plugins directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/plugins/
    DESTINATION plugins
    OPTIONAL
)
