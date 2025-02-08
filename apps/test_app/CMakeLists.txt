cmake_minimum_required(VERSION 3.5)
set(APP_NAME synodia)
project(${APP_NAME})

add_executable(${APP_NAME}
    main.cpp

    gameobjects/player.cpp
    gameobjects/player.h

    gameobjects/nonplayable.cpp
    gameobjects/nonplayable.h
    gameobjects/stationary.cpp
    gameobjects/stationary.h

    gui/menubutton.h
    gui/menubutton.cpp
    gui/textbutton.h
    gui/textbutton.cpp
    gui/progressBar.h
    gui/progressBar.cpp

    gameobjects/gamescene.h
    gameobjects/gamescene.cpp

    states/pausestate.h
    states/playstate.h
    states/menustate.h
    states/winstate.h
    states/losestate.h
    states/playstate.cpp
    states/menustate.cpp
    states/pausestate.cpp
    states/winstate.cpp
    states/losestate.cpp

    gameobjects/speech.cpp
    gameobjects/speech.h)

set(EXECUTABLE_DIR ${CMAKE_BINARY_DIR}/apps/${APP_NAME}/$<CONFIG>)
set_target_properties(${APP_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_DIR}
)

include (${CMAKE_SOURCE_DIR}/cmake/Deploy.cmake)

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
