### THIRD PARTY LIBRARIES
set(CMAKE_WARN_DEPRECATED OFF)

### SDL
if (NOT BUILD_SHARED_LIBS)
  set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "Don't build SDL as shared lib")
endif()
option(SDL_TEST "Build the SDL3_test library" OFF)
add_subdirectory(SDL)

### SDL_image
option(SDL3IMAGE_PNG "Support loading PNG images" ON)
option(SDL3IMAGE_WEBP "Support loading WEBP images" ON)
option(SDL3IMAGE_SAMPLES "Build the SDL3_image sample program(s)" OFF)
option(SDL3IMAGE_INSTALL "Enable SDL3_image install target" OFF)
option(SDL3IMAGE_DEPS_SHARED "Load dependencies dynamically" OFF)
option(SDL3IMAGE_SAMPLES "Build the SDL3_image sample program(s)" OFF)
option(SDL3IMAGE_BMP "Support loading BMP images" OFF)
option(SDL3IMAGE_GIF "Support loading GIF images" OFF)
option(SDL3IMAGE_JPG "Support loading JPEG images" OFF)
option(SDL3IMAGE_LBM "Support loading LBM images" OFF)
option(SDL3IMAGE_PCX "Support loading PCX images" OFF)
option(SDL3IMAGE_PNM "Support loading PNM images" OFF)
option(SDL3IMAGE_QOI "Support loading QOI images" OFF)
option(SDL3IMAGE_SVG "Support loading SVG images" OFF)
option(SDL3IMAGE_TGA "Support loading TGA images" OFF)
option(SDL3IMAGE_XCF "Support loading XCF images" OFF)
option(SDL3IMAGE_XPM "Support loading XPM images" OFF)
option(SDL3IMAGE_XV "Support loading XV images" OFF)

set(SDL3IMAGE_WEBP ON CACHE BOOL "Support loading WEBP images")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(SDL_image)
# To quiet down "target X has PUBLIC_HEADER files but no PUBLIC_HEADER DESTINATION"
set_target_properties(SDL3_image PROPERTIES
  PUBLIC_HEADER ""
)

### SDL_ttf
option(SDL3TTF_SAMPLES "Build the SDL3_ttf sample program(s)" OFF)
option(SDL3TTF_INSTALL "Enable SDL3_ttf install target" OFF)
option(SDL3TTF_VENDORED "Use vendored third-party libraries" ON)
add_subdirectory(SDL_ttf)
# To quiet down "target X has PUBLIC_HEADER files but no PUBLIC_HEADER DESTINATION"
set_target_properties(SDL3_ttf PROPERTIES
  PUBLIC_HEADER ""
)

### SDL_Mixer
option(SDL3MIXER_INSTALL "Enable SDL3mixer install target" OFF)
option(SDL3MIXER_DEPS_SHARED "Default value for loading dependencies dynamically" OFF)
option(SDL3MIXER_SAMPLES "Build the SDL3_mixer sample program(s)" OFF)
option(SDL3MIXER_CMD "Support an external music player" OFF)
option(SDL3MIXER_FLAC "Enable FLAC music" OFF)
option(SDL3MIXER_MOD "Support loading MOD music" OFF)
option(SDL3MIXER_MP3 "Enable MP3 music" OFF)
option(SDL3MIXER_MIDI "Enable MIDI music" OFF)
option(SDL3MIXER_OPUS "Enable Opus music" OFF)

add_subdirectory(SDL_mixer)
# To quiet down "target X has PUBLIC_HEADER files but no PUBLIC_HEADER DESTINATION"
set_target_properties(SDL3_mixer PROPERTIES
  PUBLIC_HEADER ""
)

## inipp 
add_subdirectory(inipp)

## nlohmann json
add_subdirectory(json)

## fmtlib
add_subdirectory(fmt)

### imgui
set(imgui_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui.h
  ${imgui_SOURCE_DIR}/imconfig.h
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_internal.h
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imstb_rectpack.h
  ${imgui_SOURCE_DIR}/imstb_textedit.h
  ${imgui_SOURCE_DIR}/imstb_truetype.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC SDL3::SDL3)

### GLM
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)
set(GLM_BUILD_TESTS OFF CACHE BOOL "Build GLM tests")
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "Build libraries off")
add_subdirectory(glm)

### Lua
file(GLOB LUA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lua/*.c)
add_library(lua STATIC ${LUA_SOURCES})
target_include_directories(lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lua/src)

### Sol2
add_library(sol2 INTERFACE)
target_include_directories(sol2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sol2/include)
target_link_libraries(sol2 INTERFACE lua)

### Glad
add_subdirectory(glad)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)


### END OF THIRD PARTY LIBRARIES
set(CMAKE_WARN_DEPRECATED ON)