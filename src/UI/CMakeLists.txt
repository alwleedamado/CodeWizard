# Auto-generated CMakeLists.txt for UI module

# --------------------------
# Public interface
# --------------------------
add_library(UI INTERFACE)

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

# Link Qt only for modules that need it
# Modify below as needed
target_link_libraries(UIImpl PRIVATE Qt6::Core Qt6::Widgets)
