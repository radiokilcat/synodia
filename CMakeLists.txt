cmake_minimum_required(VERSION 3.5)
project(synodia)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set module path for custom scripts (like Deploy.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Output directories
set(OUTPUT_DEBUG "Debug")
set(OUTPUT_REL "Release")

set(ANVIL_BIN_DIR ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ANVIL_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ANVIL_BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ANVIL_BIN_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ANVIL_BIN_DIR}/${OUTPUT_DEBUG})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ANVIL_BIN_DIR}/${OUTPUT_DEBUG})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ANVIL_BIN_DIR}/${OUTPUT_DEBUG})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ANVIL_BIN_DIR}/${OUTPUT_REL})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ANVIL_BIN_DIR}/${OUTPUT_REL})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ANVIL_BIN_DIR}/${OUTPUT_REL})

# Enable folders for IDE organization
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Enable shared or static library build
option(BUILD_SHARED_LIBS "Build shared libraries instead of static" ON)

# Add subdirectories
add_subdirectory(anvil)
add_subdirectory(apps)

message(STATUS "CMake configuration complete.")
