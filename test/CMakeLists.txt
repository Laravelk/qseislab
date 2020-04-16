set( EX_FILE  tests )
set( LIB_NAME  qseislab_lib)

set( CMAKE_CXX_STANDARD  17 )
set( CMAKE_AUTOMOC  ON )


find_package( Qt5Test  REQUIRED )

include_directories( "../main/src" )

file( GLOB_RECURSE  SOURCES  "*.cpp"  "*.c" )

foreach( TEST_FILE  ${SOURCES} )
    find_package(Eigen3  REQUIRED)
    get_filename_component( TEST_NAME  ${TEST_FILE}  NAME_WE)
    add_executable( ${TEST_NAME}  ${TEST_FILE} )
    target_link_libraries( ${TEST_NAME}  ${LIB_NAME}  Qt5::Test )
    target_link_libraries( ${TEST_NAME} ${LIB_NAME} Eigen3::Eigen)
    add_test( ${TEST_NAME}  ${TEST_NAME} )
endforeach()
