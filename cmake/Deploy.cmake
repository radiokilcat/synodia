function(copy_assets TARGET_NAME SOURCE_ASSETS SOURCE_CONFIG)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_ASSETS} $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_CONFIG} $<TARGET_FILE_DIR:${TARGET_NAME}>/
        COMMENT "Copying assets for ${TARGET_NAME}..."
    )
endfunction()
