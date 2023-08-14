file(GLOB inc_files "${INCLUDE_DIRS}/nomogramms/*.h")
file(GLOB src_files "./*.cpp")

add_library(nomogramms 
	${inc_files}
	${src_files})

find_package(Qt5 REQUIRED COMPONENTS Core)
target_link_libraries(nomogramms PRIVATE Qt5::Core PRIVATE lua PRIVATE lua_bridge PRIVATE common PUBLIC db)
target_include_directories(nomogramms PUBLIC ${EXTERNAL_INCLUDES})