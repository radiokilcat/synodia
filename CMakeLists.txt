cmake_minimum_required(VERSION 3.5)

project(synodia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#option(MYGAME_VENDORED "Use vendored libraries" OFF)

#if(MYGAME_VENDORED)
#    add_subdirectory("C:/SDL" EXCLUDE_FROM_ALL)
#else()
#    # 1. Look for a SDL3 package, 2. look for the SDL3 component and 3. fail if none can be found
#    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
#endif()

#find_package(SDL2_image REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
#target_link_libraries(Exe ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(WIN32)
 set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message("cxx flags:" ${CMAKE_CXX_FLAGS})
add_subdirectory(anvil)
add_subdirectory(apps)

