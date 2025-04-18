# Try to detect SDL3::SDL3 if it was added via add_subdirectory
if (TARGET SDL3::SDL3)
    set(SDL3_FOUND TRUE)
    get_target_property(SDL3_INCLUDE_DIRS SDL3::SDL3 INTERFACE_INCLUDE_DIRECTORIES)
    set(SDL3_LIBRARIES SDL3::SDL3)
    return()
endif()

# Otherwise fallback to manual search
find_path(SDL3_INCLUDE_DIR SDL3/SDL.h)
find_library(SDL3_LIBRARY NAMES SDL3 SDL3-static)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL3
    REQUIRED_VARS SDL3_LIBRARY SDL3_INCLUDE_DIR
)

if (SDL3_FOUND)
    set(SDL3_INCLUDE_DIRS ${SDL3_INCLUDE_DIR})
    set(SDL3_LIBRARIES ${SDL3_LIBRARY})
endif()
