cmake_minimum_required(VERSION 3.14)
project(qseislab)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(_Q_MODULES
    Widgets
    Core
    DataVisualization
    Charts
)

#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")
find_package(Qt5 COMPONENTS ${_Q_MODULES} REQUIRED)


# Bad way
set(Boost_INCLUDE_DIR C:/Users/seismuser/all/boost_1_71_0/boost_1_71_0)
find_package(Boost REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(segyio REQUIRED)


set(SOURCES
        event_operation/add_event/controller.cpp
        event_operation/add_event/model.cpp
        event_operation/add_event/view/filemanager.cpp
        event_operation/add_event/view/wellmanager.cpp
        event_operation/add_event/view/view.cpp

        event_operation/view_event/controller.cpp
        event_operation/view_event/view.cpp

        event_operation/share_view/view/view.cpp
        event_operation/share_view/view/wavepick.cpp
        event_operation/share_view/infoevent.cpp
        event_operation/share_view/controller.cpp
        event_operation/share_view/chartgesture.cpp


        horizon_operation/controller.cpp
        horizon_operation/model.cpp
        horizon_operation/view/view.cpp
        horizon_operation/view/addhorizonmanager.cpp
        horizon_operation/view/filemanager.cpp


        receiver_operation/controller.cpp
        receiver_operation/model.cpp
        receiver_operation/CSVparser.cpp
        receiver_operation/view/view.cpp
        receiver_operation/view/inforeceiver.cpp
        receiver_operation/view/addreceivermanager.cpp


        well_operation/controller.cpp
        well_operation/model.cpp
        well_operation/view/view.cpp
        well_operation/view/addwellmanager.cpp
        well_operation/view/filemanager.cpp


        data/seismproject.cpp
        data/seismcomponent.cpp
        data/seismwavepick.cpp
        data/seismevent.cpp
        data/seismtrace.cpp
        data/seismhorizon.cpp
        data/seismwell.cpp
        data/seismreceiver.cpp
        data/seismchannelreceiver.cpp
        data/io/segyreader.cpp
        data/io/seismcomponentreader.cpp
        data/io/seismcomponentwriter.cpp
        data/io/pointreader.cpp
        data/io/pointwriter.cpp

        main.cpp

        main/controller.cpp
        main/view/view.cpp
        main/view/startpage.cpp
        main/view/workpage.cpp
        main/view/infoproject.cpp
        main/view/tableassistant.cpp
        main/surface_view/surface.cpp

        project_operation/new_project/controller.cpp
        project_operation/new_project/view.cpp

        project_operation/share_view/infoproject.cpp
        project_operation/share_view/filemanager.cpp

        project_operation/open_project/controller.cpp
        project_operation/open_project/view.cpp
        project_operation/open_project/model.cpp

        project_operation/save_project/controller.cpp

        project_operation/close_project/controller.cpp
)

set(HEADERS
        event_operation/add_event/controller.h
        event_operation/add_event/model.h
        event_operation/add_event/view/filemanager.h
        event_operation/add_event/view/wellmanager.h
        event_operation/add_event/view/view.h

        event_operation/view_event/controller.h
        event_operation/view_event/view.h

        event_operation/share_view/infoevent.h
        event_operation/share_view/controller.h
        event_operation/share_view/chartgesture.h
        event_operation/share_view/view/view.h
        event_operation/share_view/view/wavepick.h


        horizon_operation/controller.h
        horizon_operation/model.h
        horizon_operation/view/view.h
        horizon_operation/view/addhorizonmanager.h
        horizon_operation/view/filemanager.h


        receiver_operation/controller.h
        receiver_operation/model.h
        receiver_operation/CSVparser.hpp
        receiver_operation/view/view.h
        receiver_operation/view/inforeceiver.h
        receiver_operation/view/addreceivermanager.h


        well_operation/controller.h
        well_operation/model.h
        well_operation/view/view.h
        well_operation/view/addwellmanager.h
        well_operation/view/filemanager.h


        data/seism_data_type.h
        data/seismproject.h
        data/seismcomponent.h
        data/seismwavepick.h
        data/seismevent.h
        data/seismtrace.h
        data/seismhorizon.h
        data/seismwell.h
        data/seismreceiver.h
        data/seismchannelreceiver.h
        data/io/segy_files_params.h
        data/io/abstractsegyreader.h
        data/io/segyreader.h
        data/io/seismcomponentreader.h
        data/io/seismcomponentwriter.h
        data/io/pointreader.h
        data/io/pointwriter.h

        main/controller.h
        main/view/view.h
        main/view/startpage.h
        main/view/workpage.h
        main/view/infoproject.h
        main/view/tableassistant.h
        main/view/parser.h
        main/surface_view/surface.h

        project_operation/new_project/controller.h
        project_operation/new_project/view.h

        project_operation/share_view/infoproject.h
        project_operation/share_view/filemanager.h

        project_operation/open_project/controller.h
        project_operation/open_project/view.h
        project_operation/open_project/model.h

        project_operation/save_project/controller.h

        project_operation/close_project/controller.h
)

add_executable(qseislab ${SOURCES} ${HEADERS} resources/resources.qrc)

qt5_use_modules(qseislab ${_Q_MODULES})

target_link_libraries(qseislab segyio::segyio)
