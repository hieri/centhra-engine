add_subdirectory(ce_base)
if(CE_CLIENT)
	add_subdirectory(ce_frontend)
	add_subdirectory(ce_ui)
endif()
add_subdirectory(ce_game2d)
# Note: We are ignoring Game 3D for now.
# add_subdirectory(ce_game3d)
