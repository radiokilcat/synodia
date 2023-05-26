add_executable(cmake-excercise main.cpp)

#install(TARGETS cmake-excercise
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

target_link_libraries(cmake-excercise ${SDL2_LIBRARIES} )
