cmake_minimum_required(VERSION 3.21)

add_executable(example main.cpp)
# Apply WASM-specific configuration
if(EMSCRIPTEN)
  myproject_configure_wasm_target(example
    TITLE "Example Sample"
    DESCRIPTION "example terminal application"
  )
endif()
