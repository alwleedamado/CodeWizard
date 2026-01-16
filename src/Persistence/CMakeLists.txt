# --------------------------
# Public interface
# --------------------------
add_library(Persistence INTERFACE)

target_link_libraries(Persistence
    INTERFACE Services
)

target_include_directories(Persistence
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------
# Private implementation
# --------------------------
add_library(PersistenceImpl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/PersistenceImpl.cpp
)

target_include_directories(PersistenceImpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(PersistenceImpl
    PRIVATE Persistence
    PRIVATE Services
)
