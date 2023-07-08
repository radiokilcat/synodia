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
    ${CMAKE_BINARY_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}/settings.ini COPYONLY)

file(COPY res
     DESTINATION ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(test_app PUBLIC anvil)
