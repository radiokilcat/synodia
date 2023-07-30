add_executable(test_app
    main.cpp

    gameobjects/player.cpp
    gameobjects/player.h

    gameobjects/nonplayable.cpp
    gameobjects/nonplayable.h

    gui/menubutton.h
    gui/menubutton.cpp
    gui/textbutton.h
    gui/textbutton.cpp

    gameobjects/gamescene.h
    gameobjects/gamescene.cpp
    states/pausestate.h
    states/playstate.h
    states/menustate.h

    states/pausestate.cpp
    states/playstate.cpp
    states/menustate.cpp
    gameobjects/speech.cpp
    gameobjects/speech.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/settings.ini
    ${CMAKE_BINARY_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/settings.ini COPYONLY)

file(COPY res
     DESTINATION ${CMAKE_BINARY_DIR}/bin/)

target_link_libraries(test_app PUBLIC anvil)
