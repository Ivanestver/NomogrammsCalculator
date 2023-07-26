add_subdirectory(lua)
add_subdirectory(LuaBridge)

set(EXTERNAL_LIBS lua lua_bridge PARENT_SCOPE)
set(EXTERNAL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/lua/include" "${CMAKE_CURRENT_SOURCE_DIR}/LuaBridge" PARENT_SCOPE)