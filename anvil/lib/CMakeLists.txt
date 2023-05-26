#cmake_minimum_required(VERSION 3.5)

#project(cmake-excercise LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_executable(cmake-excercise main.cpp)

#install(TARGETS cmake-excercise
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(automaton test.cpp test.h )
