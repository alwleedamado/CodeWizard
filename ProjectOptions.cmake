include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(CodeWizard_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)

    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(CodeWizard_setup_options)

  option(CodeWizard_ENABLE_HARDENING "Enable hardening" ON)
  option(CodeWizard_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    CodeWizard_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    CodeWizard_ENABLE_HARDENING
    OFF)

  CodeWizard_supports_sanitizers()

  option(CodeWizard_ENABLE_IPO "Enable IPO/LTO" ON)
  option(CodeWizard_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
  option(CodeWizard_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
  option(CodeWizard_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
  option(CodeWizard_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
  option(CodeWizard_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
  option(CodeWizard_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
  option(CodeWizard_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
  option(CodeWizard_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
  option(CodeWizard_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
  option(CodeWizard_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
  option(CodeWizard_ENABLE_PCH "Enable precompiled headers" OFF)
  option(CodeWizard_ENABLE_CACHE "Enable ccache" ON)

  CodeWizard_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (CodeWizard_ENABLE_SANITIZER_ADDRESS OR CodeWizard_ENABLE_SANITIZER_THREAD OR CodeWizard_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(CodeWizard_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(CodeWizard_global_options)
  if(CodeWizard_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    CodeWizard_enable_ipo()
  endif()

  CodeWizard_supports_sanitizers()

  if(CodeWizard_ENABLE_HARDENING AND CodeWizard_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR CodeWizard_ENABLE_SANITIZER_UNDEFINED
       OR CodeWizard_ENABLE_SANITIZER_ADDRESS
       OR CodeWizard_ENABLE_SANITIZER_THREAD
       OR CodeWizard_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${CodeWizard_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${CodeWizard_ENABLE_SANITIZER_UNDEFINED}")
    CodeWizard_enable_hardening(CodeWizard_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(CodeWizard_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(CodeWizard_warnings INTERFACE)
  add_library(CodeWizard_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  CodeWizard_set_project_warnings(
    CodeWizard_warnings
    ${CodeWizard_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(CodeWizard_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    CodeWizard_configure_linker(CodeWizard_options)
  endif()

  include(cmake/Sanitizers.cmake)
  CodeWizard_enable_sanitizers(
    CodeWizard_options
    ${CodeWizard_ENABLE_SANITIZER_ADDRESS}
    ${CodeWizard_ENABLE_SANITIZER_LEAK}
    ${CodeWizard_ENABLE_SANITIZER_UNDEFINED}
    ${CodeWizard_ENABLE_SANITIZER_THREAD}
    ${CodeWizard_ENABLE_SANITIZER_MEMORY})

  set_target_properties(CodeWizard_options PROPERTIES UNITY_BUILD ${CodeWizard_ENABLE_UNITY_BUILD})

  if(CodeWizard_ENABLE_PCH)
    target_precompile_headers(
      CodeWizard_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(CodeWizard_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    CodeWizard_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(CodeWizard_ENABLE_CLANG_TIDY)
    CodeWizard_enable_clang_tidy(CodeWizard_options ${CodeWizard_WARNINGS_AS_ERRORS})
  endif()

  if(CodeWizard_ENABLE_CPPCHECK)
    CodeWizard_enable_cppcheck(${CodeWizard_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(CodeWizard_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    CodeWizard_enable_coverage(CodeWizard_options)
  endif()

  if(CodeWizard_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(CodeWizard_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(CodeWizard_ENABLE_HARDENING AND NOT CodeWizard_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR CodeWizard_ENABLE_SANITIZER_UNDEFINED
       OR CodeWizard_ENABLE_SANITIZER_ADDRESS
       OR CodeWizard_ENABLE_SANITIZER_THREAD
       OR CodeWizard_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    CodeWizard_enable_hardening(CodeWizard_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
