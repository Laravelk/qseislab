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
        add_event/controller.cpp
        add_event/model.cpp
        add_event/view/filemanager.cpp
        add_event/view/infoevent.cpp
#        add_event/view/simulationgraphic.cpp
        add_event/view/view.cpp
        add_event/view/chart.cpp
        add_event/view/chartview.cpp
        add_event/view/graphicevent.cpp

        add_horizon/controller.cpp
        add_horizon/model.cpp

        data/seismproject.cpp
        data/seismcomponent.cpp
        data/seismevent.cpp
        data/seismtrace.cpp
        data/seismpoint.cpp
        data/seismhorizon.cpp
        data/io/segyreader.cpp
        data/io/seismcomponentreader.cpp
        data/io/seismcomponentwriter.cpp
        data/io/seismpointreader.cpp
        data/io/seismpointwriter.cpp

        main.cpp

        main/controller.cpp
        main/scatterdatamodifier.cpp
        main/custominputhandler.cpp
        main/view/view.cpp
        main/view/startpage.cpp
        main/view/workpage.cpp
        main/view/infoproject.cpp

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
        add_event/controller.h
        add_event/model.h
        add_event/view/filemanager.h
        add_event/view/infoevent.h
#        add_event/view/simulationgraphic.h
        add_event/view/view.h
        add_event/view/chart.h
        add_event/view/chartview.h
        add_event/view/graphicevent.h

        add_horizon/controller.h
        add_horizon/model.h

        data/seismproject.h
        data/seismcomponent.h
        data/seismevent.h
        data/seismtrace.h
        data/seismpoint.h
        data/seismhorizon.h
        data/io/abstractsegyreader.h
        data/io/segyreader.h
        data/io/seismcomponentreader.h
        data/io/seismcomponentwriter.h
        data/io/seismpointreader.h
        data/io/seismpointwriter.h

        main/controller.h
        main/scatterdatamodifier.h
        main/custominputhandler.h
        main/view/view.h
        main/view/startpage.h
        main/view/workpage.h
        main/view/infoproject.h

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
