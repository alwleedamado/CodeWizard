# --------------------------
# Public interface
# --------------------------
add_library(Project INTERFACE)

target_link_libraries(Project
    INTERFACE Services
    INTERFACE Platform
)

target_include_directories(Project
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(ProjectImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/ProjectImpl.cpp
)

target_include_directories(ProjectImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(ProjectImpl
    PRIVATE Project
    PRIVATE Services
    PRIVATE Platform
)
