# Catch2 headless test for Persistence
add_executable(PersistenceTests
    PersistenceTest.cpp
)

target_include_directories(PersistenceTests
    PRIVATE
        ../..
        ../../src/Persistence/include
)

# Link module implementation if needed
target_link_libraries(PersistenceTests PRIVATE PersistenceImpl)
