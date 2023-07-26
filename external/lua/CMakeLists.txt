add_library(lua SHARED IMPORTED GLOBAL)
set_property(TARGET lua PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/lua54.dll")
set_property(TARGET lua PROPERTY
             IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/lua54.lib")