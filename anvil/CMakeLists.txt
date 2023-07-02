add_subdirectory(third_party)

if(WIN32)
 set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_library(anvil
  src/test.cpp
  src/test.h
  src/window.cpp
  src/application.cpp
  src/renderer.cpp
  src/texturemanager.cpp
  src/configuration.cpp
  src/ini_settings_accessor.cpp
  src/inputhandler.cpp
  src/tilemap.cpp
  include/anvil.h
  include/window.h
  include/application.h
  include/renderer.h
  include/texturemanager.h
  include/gameobject.h
  include/vector2d.h
  include/configuration.h
  include/settings_accessor.h
  include/ini_settings_accessor.h
  include/inputhandler.h
  include/tilemap.h
)

set(public_headers
    include/anvil.h
    include/window.h
)

set_target_properties(anvil PROPERTIES PUBLIC_HEADER "${public_headers}")

target_include_directories(anvil
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(BUILD_SHARED_LIBS)
 target_link_libraries(anvil 
    PRIVATE 
      inipp
    PUBLIC
      nlohmann_json
      SDL3::SDL3
      SDL3_image::SDL3_image
      SDL3_ttf::SDL3_ttf
      SDL3_mixer::SDL3_mixer
 )
else()
 target_link_libraries(anvil
    PRIVATE 
      inipp
    PUBLIC
      nlohmann_json
      SDL3::SDL3-static
      SDL3_image::SDL3_image-static
      SDL3_ttf::SDL3_ttf-static
      SDL3_mixer::SDL3_mixer-static
 )
endif()
