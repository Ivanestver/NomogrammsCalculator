file(GLOB inc_files "./*.h")
file(GLOB src_files "./*.cpp")

add_library(db_state ${inc_files} ${src_files})

find_package(Qt5 COMPONENTS Core Widgets)
target_link_libraries(db_state PUBLIC Qt5::Core PRIVATE xml)
target_include_directories(db_state PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)