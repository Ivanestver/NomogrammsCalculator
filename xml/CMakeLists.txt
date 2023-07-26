add_library(xml xml.h)

find_package(Qt5 COMPONENTS Core Widgets)
target_link_libraries(xml PUBLIC Qt5::Core)