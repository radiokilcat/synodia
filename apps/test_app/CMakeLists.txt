add_executable(test_app
    main.cpp
    player.cpp
    nonplayable.cpp
    player.h
    nonplayable.h
    game_scene.h
    game_scene.cpp
    )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/settings.ini
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

target_link_libraries(test_app PUBLIC anvil)
