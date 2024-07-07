add_executable(editor
    main.cpp
    states/mainstate.h
    states/mainstate.cpp)

include (${CMAKE_SOURCE_DIR}/cmake/Deploy.cmake)

target_link_libraries(editor PUBLIC anvil)
