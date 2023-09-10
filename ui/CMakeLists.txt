file(GLOB inc_files "./*.h")
file(GLOB src_files "./*.cpp")
file(GLOB ui_files "./ui/*.ui")
file(GLOB rc_files "./ui/*.qrc")

add_library(ui 
	${ui_files} 
	${inc_files} 
	${src_files} 
	${rc_files})

# Add the Qt5 Widgets for linking
find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Widgets)
target_link_libraries(ui PRIVATE Qt5::Core PRIVATE Qt5::Widgets PRIVATE nomogramms)
target_include_directories(ui PUBLIC "${CMAKE_BINARY_DIR}/ui/ui_autogen/include_Debug")
include_directories(ui PUBLIC "${TORCH_INSTALL_PREFIX}/include/torch/csrc/api/include/" PUBLIC "${TORCH_INSTALL_PREFIX}/include/")