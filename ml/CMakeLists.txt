file(GLOB inc_files "./*.h")
file(GLOB src_files "./*.cpp")

# Add libtorch
find_package(Torch REQUIRED)
if (Torch_FOUND)
    message("-- Torch is found")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_library(ml ${inc_files} ${src_files})

find_package(Qt5 COMPONENTS Core Widgets)
target_link_libraries(ml PUBLIC Qt5::Core PUBLIC "${TORCH_LIBRARIES}")
target_include_directories(ml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. INTERFACE "${TORCH_INSTALL_PREFIX}/include/torch/csrc/api/include/")
include_directories(ml "${TORCH_INSTALL_PREFIX}/include/torch/csrc/api/include/")
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET ml
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:ml>)
endif (MSVC)