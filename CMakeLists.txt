cmake_minimum_required(VERSION 3.14)
project(qseislab)

set(CMAKE_CXX_STANDARD 14)

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

find_package(Qt5 COMPONENTS ${_Q_MODULES} REQUIRED)


find_package(segyio REQUIRED)

set(SOURCES
        event_operation/add_event/controller.cpp
        event_operation/add_event/model.cpp
        event_operation/add_event/view/filemanager.cpp
        event_operation/add_event/view/view.cpp

        event_operation/view_event/controller.cpp
        event_operation/view_event/view.cpp

        event_operation/share_view/infoevent.cpp
        event_operation/share_view/chart.cpp
        event_operation/share_view/chartview.cpp
        event_operation/share_view/graphicevent.cpp


        horizon_operation/controller.cpp
        horizon_operation/model.cpp
        horizon_operation/view/view.cpp

        data/seismproject.cpp
        data/seismcomponent.cpp
        data/seismevent.cpp
        data/seismtrace.cpp
        data/seismhorizon.cpp
        data/io/segyreader.cpp
        data/io/seismcomponentreader.cpp
        data/io/seismcomponentwriter.cpp
        data/io/seismpointreader.cpp
        data/io/seismpointwriter.cpp

        main.cpp

        main/surface_view/surface.cpp

        main/controller.cpp
        main/view/view.cpp
        main/view/startpage.cpp
        main/view/workpage.cpp
        main/view/infoproject.cpp

        project_operation/new_project/controller.cpp
        project_operation/new_project/view.cpp
        horizon_operation/view/addhorizonmanager.cpp
        horizon_operation/view/filemanager.cpp

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
        event_operation/add_event/view/view.h

        event_operation/view_event/controller.h
        event_operation/view_event/view.h

        event_operation/share_view/infoevent.h
        event_operation/share_view/chart.h
        event_operation/share_view/chartview.h
        event_operation/share_view/graphicevent.h

        horizon_operation/controller.h
        horizon_operation/model.h

        horizon_operation/view/addhorizonmanager.h
        horizon_operation/view/filemanager.h
        horizon_operation/view/view.h

        data/seismproject.h
        data/seismcomponent.h
        data/seismevent.h
        data/seismtrace.h
        data/seismhorizon.h
        data/io/abstractsegyreader.h
        data/io/segyreader.h
        data/io/seismcomponentreader.h
        data/io/seismcomponentwriter.h
        data/io/seismpointreader.h
        data/io/seismpointwriter.h
        data/io/defines.h

        main/surface_view/surface.h

        main/controller.h
        main/view/view.h
        main/view/startpage.h
        main/view/workpage.h
        main/view/infoproject.h

        resources/1.h


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

add_executable(qseislab ${SOURCES} ${HEADERS})

qt5_use_modules(qseislab ${_Q_MODULES})

target_link_libraries(qseislab segyio::segyio)
