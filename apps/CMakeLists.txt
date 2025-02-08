cmake_minimum_required(VERSION 3.5)
project(apps)

# Get all subdirectories in apps/
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(subdir ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        add_subdirectory(${subdir})
    endif()
endforeach()
