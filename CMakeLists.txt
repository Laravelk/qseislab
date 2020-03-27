cmake_minimum_required( VERSION  3.10 )

project( qseislab_full )

add_subdirectory( main )
enable_testing( true )
add_subdirectory( test )
