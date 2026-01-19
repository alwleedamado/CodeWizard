# tests/CMakeLists.txt
include(FetchContent)

# Discover and add all test subdirectories
file(GLOB TEST_SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(TEST_SUBDIR ${TEST_SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SUBDIR})
        add_subdirectory(${TEST_SUBDIR})
    endif()
endforeach()
