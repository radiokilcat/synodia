function(copy_assets TARGET_NAME SOURCE_ASSETS SOURCE_CONFIG)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_ASSETS} $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_CONFIG} $<TARGET_FILE_DIR:${TARGET_NAME}>/
        COMMENT "Copying assets for ${TARGET_NAME}..."
    )
endfunction()

function(copy_anvil_dlls TARGET_NAME)
    if(NOT BUILD_SHARED_LIBS)
        return()
    endif()
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/anvil.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3_image.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3_ttf.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/SDL3_mixer.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/fmt$<$<CONFIG:Debug>:d>.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/libwebp.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/libwebpdemux.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/libwebpmux.dll
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMENT "Copying DLLs for ${TARGET_NAME}..."
    )
endfunction()
