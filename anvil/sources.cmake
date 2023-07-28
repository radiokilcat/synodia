
set(SOURCES
  src/test.cpp
  src/window.cpp
  src/application.cpp
  src/renderer.cpp
  src/texturemanager.cpp
  src/configuration.cpp
  src/ini_settings_accessor.cpp
  src/inputhandler.cpp
  src/tilemap.cpp
  src/game_state_machine.cpp
  src/utils.cpp
  src/isogameobject.cpp
  src/textlabel.cpp
  src/fontloader.cpp
  src/json_serializer.cpp
)

set(HEADERS
  include/anvil.h
  include/utils.h
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
  include/gamestate.h
  include/game_state_machine.h
  include/isogameobject.h
  include/textlabel.h
  include/fontloader.h
  include/json_serializer/json_serializer.h
  include/json_serializer/serializable_base.h
)

set(public_headers include/anvil.h )

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})
