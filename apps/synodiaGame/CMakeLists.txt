cmake_minimum_required(VERSION 3.5)
set(APP_NAME synodiaGame)
project(${APP_NAME})

add_executable(${APP_NAME} main.cpp)

set(EXECUTABLE_DIR ${CMAKE_BINARY_DIR}/apps/${APP_NAME}/$<CONFIG>)

set_target_properties(${APP_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_DIR}
    WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)

include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/Deploy.cmake")

copy_assets(
    ${APP_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_CURRENT_SOURCE_DIR}/settings.ini"
)

target_link_libraries(${APP_NAME} PUBLIC anvil)

copy_anvil_dlls(${APP_NAME})
