cmake_minimum_required( VERSION  3.10 )

set( EX_FILE   qseislab )
set( LIB_NAME  qseislab_lib )

set( CMAKE_CXX_STANDARD  17 )

set( CMAKE_INCLUDE_CURRENT_DIR  ON )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set( _Q_MODULES
     Widgets
     Core
     DataVisualization
     Charts
     3DCore
     3DExtras
     3DRender
     3DInput
     3DAnimation
)
find_package( Qt5  COMPONENTS  ${_Q_MODULES}  REQUIRED )

find_package( Boost  1.60.0  REQUIRED )
if( Boost_FOUND )
  include_directories( ${Boost_INCLUDE_DIRS} )
endif()

find_package( segyio  REQUIRED )
find_package( Eigen3  REQUIRED )


set ( RESOURCES     resources/resources.qrc )
file( GLOB_RECURSE  HEADERS  "*.hpp"  "*.h" )
file( GLOB_RECURSE  SOURCES  "*.cpp"  "*.c" !"main.cpp" )


add_library( ${LIB_NAME}  ${HEADERS}  ${SOURCES} )
qt5_use_modules( ${LIB_NAME}  ${_Q_MODULES} )
target_link_libraries( ${LIB_NAME}  segyio::segyio )
target_link_libraries( ${LIB_NAME}  Eigen3::Eigen )


add_executable( ${EX_FILE}  ${RESOURCES}  main.cpp )
target_link_libraries( ${EX_FILE}   ${LIB_NAME} )
