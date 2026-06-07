set(SOURCE_FOLDERS
    "src"
    "src/ui"
    "src/components"
    "src/game_objects"
    "src/json_serializer"
    "src/states"
)

if(ANVIL_IMGUI)
    list(APPEND SOURCE_FOLDERS "src/AnvilImgui")
endif()


set(SOURCES "")
set(HEADERS "")

foreach(FOLDER ${SOURCE_FOLDERS})
    file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.cpp")
    file(GLOB_RECURSE H_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.h")

    list(APPEND SOURCES ${CPP_FILES})
    list(APPEND HEADERS ${H_FILES})
endforeach()

if(NOT ANVIL_IMGUI)
    list(FILTER SOURCES EXCLUDE REGEX ".*/AnvilImgui/.*")
    list(FILTER HEADERS EXCLUDE REGEX ".*/AnvilImgui/.*")
endif()

set(public_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/anvil.h")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCES} ${HEADERS})
