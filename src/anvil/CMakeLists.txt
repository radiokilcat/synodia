add_library(anvil
  anvil.h
  test.cpp
  test.h
  window.cpp
  window.h
)

if (BUILD_SHARED_LIBS)
  set_target_properties(anvil PROPERTIES
    INSTALL_RPATH $ORIGIN
  )
endif()

target_include_directories(anvil
  PRIVATE
    ${SDL3_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
)

if(BUILD_SHARED_LIBS)
  add_definitions(-DEXPORT_DLL)
  target_link_libraries(anvil PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
  )
else()
  target_link_libraries(anvil PUBLIC
    SDL3::SDL3-static
    SDL3_image::SDL3_image-static
    SDL3_ttf::SDL3_ttf-static
    SDL3_mixer::SDL3_mixer-static
  )
endif()
