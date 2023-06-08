add_executable(test_app main.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/settings.ini
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
#install(TARGETS cmake-excercise
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

target_link_libraries(test_app PUBLIC anvil)
