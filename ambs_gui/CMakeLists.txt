cmake_minimum_required(VERSION 3.0.2)
project(ambs_gui)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  ambs_core
  ambs_msgs
  roscpp
  roslint
  rqt_gui
  rqt_gui_cpp
  std_msgs
  nodelet
)

## System dependencies are found with CMake's conventions

if("${qt_gui_cpp_USE_QT_MAJOR_VERSION} " STREQUAL "5 ")
  find_package(Qt5Widgets REQUIRED)
else()
  find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
  include(${QT_USE_FILE})
endif()

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
catkin_python_setup()

roslint_cpp()

catkin_package(
#  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
  roscpp
  rqt_gui_cpp
  std_msgs
  ambs_core
  ambs_msgs
#  DEPENDS system_lib
)


set (ambs_gui_SRCS
  src/ambs_gui/my_plugin.cpp
  src/ambs_gui/tablemodel.cpp
)

set(ambs_gui_HDRS
  include/ambs_gui/my_plugin.h
  include/ambs_gui/tablemodel.h
)

set(ambs_gui_UIS
  src/ambs_gui/my_plugin.ui
)
# changed _MOCS for _SRCS
if("${qt_gui_cpp_USE_QT_MAJOR_VERSION} " STREQUAL "5 ")
  qt5_wrap_cpp(ambs_gui_MOCS ${ambs_gui_HDRS})
  qt5_wrap_ui(ambs_gui_UIS_H ${ambs_gui_UIS})
else()
  qt4_wrap_cpp(ambs_gui_MOCS ${ambs_gui_HDRS})
  qt4_wrap_ui(ambs_gui_UIS_H ${ambs_gui_UIS})
endif()

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  # TODO(lucasw) This is needed for generated headers, but catkin lint doesn't like it
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${catkin_INCLUDE_DIRS}
  include/ambs_gui/
)
# changed MOCS for HDRS
## Declare a cpp library
add_library(${PROJECT_NAME}
    ${ambs_gui_SRCS}
    ${ambs_gui_MOCS}
    ${ambs_gui_UIS_H}
    )

## Declare a C++ executable
add_executable(test_node src/test.cpp)
target_link_libraries(test_node ${catkin_LIBRARIES})

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(rqt_example_cpp_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

if("${qt_gui_cpp_USE_QT_MAJOR_VERSION} " STREQUAL "5 ")
  target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
else()
  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
endif()


## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
install(PROGRAMS
  scripts/my_plugin
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark executables and/or libraries for installation
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(FILES
  plugin.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
