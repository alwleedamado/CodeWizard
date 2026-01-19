# Usage: include(scripts/add_headers_targets.cmake) in root CMakeLists.txt
# Call: add_headers_targets(<Module1> <Module2> ...)

function(add_headers_targets)
    foreach(module_name IN LISTS ARGN)
        set(module_dir "${CMAKE_SOURCE_DIR}/src/${module_name}")
        set(include_dir "${module_dir}/include/${module_name}")

        if(EXISTS "${include_dir}")
            file(GLOB_RECURSE module_headers "${include_dir}/*.h" "${include_dir}/*.hpp")

            if(module_headers)
                set(helper_target "${module_name}_headers")

                add_library(${helper_target} STATIC "")

                # Tell CMake it’s C++
                set_target_properties(${helper_target} PROPERTIES LINKER_LANGUAGE CXX)

                target_sources(${helper_target} PRIVATE ${module_headers})
                target_link_libraries(${helper_target} PRIVATE ${module_name})

                # Optional: group headers in IDE
                source_group("Public Headers\\${module_name}" FILES ${module_headers})

                message(STATUS "Created helper target ${helper_target} for IDE")
            else()
                message(WARNING "No headers found for module ${module_name} in ${include_dir}")
            endif()
        else()
            message(WARNING "Include directory not found for module ${module_name}: ${include_dir}")
        endif()
    endforeach()
endfunction()
