cmake_minimum_required(VERSION 3.8.2)

project(NomogrammDBEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets Gui Sql)

# There may be a way to tell up front if Qt5 is going to be found, but I haven't found
# a foolproof way to do it yet, so settle for the default error message for now.
#if(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)
#    message(WARNING "CMAKE_PREFIX_PATH is not defined, so find_package may not work. Set the CMAKE_PREFIX_PATH "
#            "environment variable to the install prefix of Qt 5, either on the command line as "
#            "-DCMAKE_PREFIX_PATH=\"path/to/Qt5/lib/cmake\" or with set(CMAKE_PREFIX_PATH path/to/Qt5/lib/cmake)")
#endif(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# Tell CMake to create the helloworld executable
file(GLOB inc_files "./*.h")
file(GLOB src_files "./*.cpp")
file(GLOB ui_files "./*.ui")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../xml" "${CMAKE_CURRENT_BINARY_DIR}/xml")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../db_state" "${CMAKE_CURRENT_BINARY_DIR}/db_state")

add_executable(NomogrammDBEditor main.cpp ${inc_files} ${src_files} ${ui_files} )
# Add the Qt5 Widgets for linking
target_link_libraries(NomogrammDBEditor PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Sql db_state)
target_include_directories(NomogrammDBEditor 
	PUBLIC ${CMAKE_BINARY_DIR} 
	PUBLIC "${CMAKE_SOURCE_DIR}/.."
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${Qt5Widgets_INCLUDES} ${Qt5_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../db_state")
add_definitions(${Qt5Widgets_DEFINITIONS})


file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")

add_custom_target(Qt5CopyBinaries
  # todo: check if debug and release folder exist
  # debug version
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGLd.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libGLESv2d.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll         ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll          ${CMAKE_BINARY_DIR}/Debug
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Declaratived.dll  ${CMAKE_BINARY_DIR}/Debug
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Networkd.dll      ${CMAKE_BINARY_DIR}/Debug
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5OpenGLd.dll       ${CMAKE_BINARY_DIR}/Debug
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Scriptd.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sqld.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll      ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Xmld.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5XmlPatternsd.dll  ${CMAKE_BINARY_DIR}/Debug
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icuin52.dll          ${CMAKE_BINARY_DIR}/Debug
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icuuc52.dll          ${CMAKE_BINARY_DIR}/Debug
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icudt52.dll          ${CMAKE_BINARY_DIR}/Debug
  
  # release version
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGL.dll           ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libGLESv2.dll        ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Core.dll          ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Gui.dll           ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Declarative.dll   ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Network.dll       ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5OpenGL.dll        ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Script.dll        ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sql.dll           ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll       ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Xml.dll           ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5XmlPatterns.dll   ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icuin52.dll          ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icuuc52.dll          ${CMAKE_BINARY_DIR}/Release
  #COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icudt52.dll          ${CMAKE_BINARY_DIR}/Release

  # Output Message
  COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
)

add_dependencies(NomogrammDBEditor Qt5CopyBinaries)