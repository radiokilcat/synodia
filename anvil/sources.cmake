
set(SOURCES
  src/test.cpp
  src/window.cpp
  src/application.cpp
  src/renderer.cpp
  src/texturemanager.cpp
  src/configuration.cpp
  src/ini_settings_accessor.cpp
  src/inputhandler.cpp
  src/game_state_machine.cpp
  src/utils.cpp
  src/fontloader.cpp
  src/json_serializer.cpp
  src/stateloader.cpp
  src/game_objects/gameobject.cpp
  src/game_objects/tilemap.cpp
  src/game_objects/isogameobject.cpp
  src/game_objects/textlabel.cpp
  src/game_objects/GameObjectsFactory.cpp
)

set(HEADERS
  include/anvil.h
  include/utils.h
  include/window.h
  include/application.h
  include/renderer.h
  include/texturemanager.h
  include/vector2d.h
  include/configuration.h
  include/settings_accessor.h
  include/ini_settings_accessor.h
  include/inputhandler.h
  include/gamestate.h
  include/game_state_machine.h
  include/fontloader.h
  include/json_serializer/json_serializer.h
  include/json_serializer/serializable_base.h
  include/json_serializer/stateloader.h
  include/game_objects/BaseGameObject.h
  include/game_objects/isogameobject.h
  include/game_objects/textlabel.h
  include/game_objects/gameobject.h
  include/game_objects/tilemap.h
  include/game_objects/GameObjectsFactory.h
)

set(public_headers include/anvil.h )

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})
