add_executable(test_app
    main.cpp

    gameobjects/player.cpp
    gameobjects/player.h

    gameobjects/nonplayable.cpp
    gameobjects/nonplayable.h

    gameobjects/menubutton.h
    gameobjects/menubutton.cpp

    gameobjects/gamescene.h
    gameobjects/gamescene.cpp
    states/pausestate.h
    states/playstate.h
    states/menustate.h

    states/pausestate.cpp
    states/playstate.cpp
    states/menustate.cpp
    )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/settings.ini
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

target_link_libraries(test_app PUBLIC anvil)
