cmake_minimum_required(VERSION 3.5)
project(anvil)

add_subdirectory(third_party)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include(sources.cmake)

add_library(anvil ${SOURCES} ${HEADERS})

set_target_properties(anvil PROPERTIES 
    PUBLIC_HEADER "${public_headers}"
    OUTPUT_NAME "anvil"
)

target_include_directories(anvil
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
if(BUILD_SHARED_LIBS)
    set(ANVIL_SDL_TARGET SDL3::SDL3)
    set(ANVIL_SDL_IMAGE_TARGET SDL3_image::SDL3_image)
    set(ANVIL_SDL_TTF_TARGET SDL3_ttf::SDL3_ttf)
    set(ANVIL_SDL_MIXER_TARGET SDL3_mixer::SDL3_mixer)
else()
    set(ANVIL_SDL_TARGET SDL3::SDL3-static)
    set(ANVIL_SDL_IMAGE_TARGET SDL3_image::SDL3_image-static)
    set(ANVIL_SDL_TTF_TARGET SDL3_ttf::SDL3_ttf-static)
    set(ANVIL_SDL_MIXER_TARGET SDL3_mixer::SDL3_mixer-static)
endif()

set(ANVIL_PUBLIC_LIBS
    ${ANVIL_SDL_TARGET}
    ${ANVIL_SDL_TTF_TARGET}
    fmt::fmt
)

set(ANVIL_PRIVATE_LIBS
    ${ANVIL_SDL_IMAGE_TARGET}
    ${ANVIL_SDL_MIXER_TARGET}
    inipp
    glm::glm-header-only
    lua
    sol2
    glad
    nlohmann_json
    imgui
)

target_link_libraries(anvil
    PUBLIC ${ANVIL_PUBLIC_LIBS}
    PRIVATE ${ANVIL_PRIVATE_LIBS}
)
