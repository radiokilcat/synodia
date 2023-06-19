add_executable(test_app
    main.cpp
    player.cpp
    nonplayable.cpp
    player.h
    nonplayable.h
    )
add_executable(test_app main.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/settings.ini
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

target_link_libraries(test_app PUBLIC anvil)
