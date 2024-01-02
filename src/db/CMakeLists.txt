file(GLOB inc_files "${INCLUDE_DIRS}/db/*.h")
file(GLOB src_files "./*.cpp")

add_library(db 
	${inc_files}
	${src_files})

find_package(Qt5 REQUIRED COMPONENTS Core Sql Gui)
target_include_directories(db PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(db PRIVATE Qt5::Core PRIVATE Qt5::Sql PRIVATE Qt5::Gui INTERFACE db_state PRIVATE xml PRIVATE exceptions)