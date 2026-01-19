# src/Services/CMakeLists.txt
add_library(Services STATIC
    src/ServiceRegistry.cpp
    src/GlobalServices.cpp
)

target_include_directories(Services PUBLIC include)
target_link_libraries(Services PUBLIC Core Platform)
target_compile_features(Services PUBLIC cxx_std_20)
