add_library(Packaging INTERFACE)
target_include_directories(Packaging
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(PackagingImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/Packager.cpp
)
target_include_directories(PackagingImpl PRIVATE internal)
#target_link_libraries(PackagingImpl PRIVATE BuildRunImpl Qt6::Core)
