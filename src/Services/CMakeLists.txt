# Auto-generated CMakeLists.txt for Services module

# --------------------------
# Public interface
# --------------------------
add_library(Services INTERFACE)

target_include_directories(Services
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(ServicesImpl STATIC
    internal/ServicesImpl.cpp
)

target_include_directories(ServicesImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
