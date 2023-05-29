add_executable(test_app main.cpp)

#install(TARGETS cmake-excercise
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

target_link_libraries(test_app PUBLIC anvil)
