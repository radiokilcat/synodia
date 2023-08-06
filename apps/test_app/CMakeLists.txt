add_executable(test_app
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


include (${CMAKE_SOURCE_DIR}/cmake/Deploy.cmake)

target_link_libraries(test_app PUBLIC anvil)
