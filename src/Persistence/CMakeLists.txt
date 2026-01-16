# Auto-generated CMakeLists.txt for Persistence module

# --------------------------
# Public interface
# --------------------------
add_library(Persistence INTERFACE)

target_include_directories(Persistence
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(PersistenceImpl STATIC
    internal/PersistenceImpl.cpp
)

target_include_directories(PersistenceImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Link Qt only for modules that need it
# Modify below as needed
