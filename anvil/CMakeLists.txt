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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# Link dependencies
if(BUILD_SHARED_LIBS)
    target_link_libraries(anvil PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        fmt::fmt
    )
else()
    target_link_libraries(anvil PUBLIC
        SDL3::SDL3-static
        SDL3_image::SDL3_image-static
        SDL3_ttf::SDL3_ttf-static
        SDL3_mixer::SDL3_mixer-static
        fmt::fmt
    )
endif()

target_link_libraries(anvil
    PRIVATE inipp glm::glm-header-only
    PRIVATE lua sol2 glad
    PUBLIC nlohmann_json imgui
)
