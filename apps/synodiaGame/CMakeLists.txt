cmake_minimum_required(VERSION 3.5)
set(APP_NAME synodiaGame)
project(${APP_NAME})

add_executable(${APP_NAME} main.cpp)

set(EXECUTABLE_DIR ${CMAKE_BINARY_DIR}/apps/${APP_NAME}/$<CONFIG>)

set_target_properties(${APP_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_DIR}
)

include(${CMAKE_SOURCE_DIR}/cmake/Deploy.cmake)

copy_assets(
    ${APP_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_CURRENT_SOURCE_DIR}/settings.ini"
)

target_link_libraries(${APP_NAME} PUBLIC anvil)

add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/anvil.dll
        $<TARGET_FILE_DIR:${APP_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3.dll
        $<TARGET_FILE_DIR:${APP_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3_image.dll
        $<TARGET_FILE_DIR:${APP_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3_ttf.dll
        $<TARGET_FILE_DIR:${APP_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3_mixer.dll
        $<TARGET_FILE_DIR:${APP_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/fmtd.dll
        $<TARGET_FILE_DIR:${APP_NAME}>
    COMMENT "Copying required DLLs to application directory..."
)
