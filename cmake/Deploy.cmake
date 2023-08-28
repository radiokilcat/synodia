set(ASSET_SOURCE_DIR "${CMAKE_SOURCE_DIR}/apps/test_app/res")

set(ASSET_DESTINATION_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}")


set(SETTINGS_SOURCE ${CMAKE_SOURCE_DIR}/apps/test_app/settings.ini)
set(SETTINGS_DESTINATION "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}")
# TODO: IN MSVC CMAKE_CFG_INTDIR won't be resolved into config name
# Seems like resoures must be included in the solution and be copied
# in compile time
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(ASSET_DESTINATION_DIR "${CMAKE_BINARY_DIR}/bin/Debug")
	set(SETTINGS_DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
endif()
file(COPY ${ASSET_SOURCE_DIR} DESTINATION ${ASSET_DESTINATION_DIR})
configure_file(${SETTINGS_SOURCE} ${SETTINGS_DESTINATION}/settings.ini COPYONLY)

