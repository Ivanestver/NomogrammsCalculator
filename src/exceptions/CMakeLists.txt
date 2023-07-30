file(GLOB inc_files "${INCLUDE_DIRS}/exceptions/*.h")
file(GLOB src_files "./*.cpp")

add_library(exceptions 
	${inc_files}
	${src_files})

find_package(Qt5 REQUIRED COMPONENTS Core)
target_link_libraries(exceptions PRIVATE Qt5::Core)