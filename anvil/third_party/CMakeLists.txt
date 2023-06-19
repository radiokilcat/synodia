### SDL
if (NOT BUILD_SHARED_LIBS)
  set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "Don't build SDL as shared lib")
endif()
option(SDL_TEST "Build the SDL3_test library" OFF)
add_subdirectory(SDL)

### SDL_image
# PNG = GOOD
option(SDL3IMAGE_PNG "Support loading PNG images" ON)

# don't need the rest, lol
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