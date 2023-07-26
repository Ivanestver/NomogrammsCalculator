file(GLOB inc_files "${INCLUDE_DIRS}/common/*.h")
file(GLOB src_files "./*.cpp")

add_library(common 
	${inc_files}
	${src_files})

find_package(Qt5 REQUIRED COMPONENTS Core)
target_link_libraries(common PRIVATE Qt5::Core)