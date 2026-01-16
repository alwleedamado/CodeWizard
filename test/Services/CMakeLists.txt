# Catch2 headless test for Services
add_executable(ServicesTests
    ServicesTest.cpp
)

target_include_directories(ServicesTests
    PRIVATE
        ../..
        ../../src/Services/include
)

# Link module implementation if needed
target_link_libraries(ServicesTests PRIVATE ServicesImpl)
