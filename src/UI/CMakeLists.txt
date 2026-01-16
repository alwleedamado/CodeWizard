# Auto-generated CMakeLists.txt for UI module

# --------------------------
# Public interface
# --------------------------
add_library(UI INTERFACE)

target_link_libraries(UI
    INTERFACE Services
    INTERFACE Platform
    INTERFACE Editor
    INTERFACE Project
)

target_include_directories(UI
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(UIImpl STATIC
    internal/UIImpl.cpp
)

target_include_directories(UIImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(UIImpl
    PRIVATE UI
    PRIVATE Services
    PRIVATE Platform
    PRIVATE Editor
    PRIVATE Project
    PRIVATE Qt6::Core
    PRIVATE Qt6::Widgets
)
