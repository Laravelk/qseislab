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
    3DCore
    3DExtras
    3Drender
    3DInput
    3DAnimation
)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
find_package(Qt5 COMPONENTS ${_Q_MODULES} REQUIRED)

#include_directories("${PROJECT_SOURCE_DIR}/parser_test")
#add_subdirectory(parser_test)

# Bad way
set(Boost_INCLUDE_DIR "C:/Users/seismuser/all/boost_1_71_0/boost_1_71_0")
find_package(Boost REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(segyio REQUIRED)

find_package(Eigen3 REQUIRED)



set(SOURCES    
        event_operation/modification/share_functions.cpp
        event_operation/modification/rotatedatatoebasis.cpp
        event_operation/modification/testmultiplier.cpp

        event_operation/one_event/controller.cpp
        event_operation/one_event/view/filemanager.cpp
        event_operation/one_event/view/wellmanager.cpp
        event_operation/one_event/view/view.cpp

        event_operation/more_events/controller.cpp
        event_operation/more_events/view/view.cpp

        event_operation/view_event/controller.cpp
        event_operation/view_event/view.cpp

        event_operation/share/polarizationanalysiscompute.cpp # test

        event_operation/share/model.cpp
        event_operation/share/view/graphic_view/graphic_view.cpp
        event_operation/share/view/graphic_view/wavepick.cpp
        event_operation/share/view/infoevent.cpp
        event_operation/share/view/graphiccontroller.cpp
        event_operation/share/view/chartgesture.cpp
        event_operation/share/view/eventtoolswidget.cpp
        event_operation/share/view/graphic_view/pipes2dname.cpp
        event_operation/share/view/3dscene/polarizationanalysiswindow.cpp


        event_operation/share/view/polar_graph/polargraph.cpp # test

        event_operation/share/view/tools_view/wigglewidget.cpp
        event_operation/share/view/tools_view/hidecomponentwidget.cpp
        event_operation/share/view/tools_view/clippingwidget.cpp
        event_operation/share/view/tools_view/gainwidget.cpp


        horizon_operation/controller.cpp
        horizon_operation/model.cpp
        horizon_operation/view/view.cpp
        horizon_operation/view/addhorizonmanager.cpp
        horizon_operation/view/filemanager.cpp


        receiver_operation/controller.cpp
        receiver_operation/model.cpp
        receiver_operation/CSVparser.cpp
        receiver_operation/view/view.cpp
        receiver_operation/view/totalchannelcounter.cpp
        receiver_operation/view/inforeceiver.cpp
        receiver_operation/view/addreceivermanager.cpp


        well_operation/controller.cpp
        well_operation/model.cpp
        well_operation/view/view.cpp
        well_operation/view/addwellmanager.cpp
        well_operation/view/filemanager.cpp


        data/seismpolarizationanalysisdata.cpp
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
        data/io/horizonreadergrd.cpp

        main.cpp
        undo_stack_work/customindividualundostack.cpp #Note: где должен лежать?
        undo_stack_work/customindividualundocommand.cpp
        undo_stack_work/shareundocommand.cpp
        undo_stack_work/event_modification/undocommandgetter.cpp
        undo_stack_work/event_modification/testindividualcommand.cpp

        main/controller.cpp
        main/view/view.cpp
        main/view/startpage.cpp
        main/view/workpage.cpp
        main/view/infoproject.cpp

        main/oil_field_scene/oilfieldscene.cpp
        main/oil_field_scene/oilfieldwidget.cpp

        share_view/tableassistant.cpp
        share_view/filteringtableassistant.cpp
        share_view/parsing/evaluateExpr.cpp

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
        event_operation/modification/share_functions.h
        event_operation/modification/rotatedatatoebasis.h
        event_operation/modification/testmultiplier.h



        event_operation/one_event/controller.h
        event_operation/one_event/view/filemanager.h
        event_operation/one_event/view/wellmanager.h
        event_operation/one_event/view/view.h


        event_operation/more_events/controller.h
        event_operation/more_events/view/view.h

        event_operation/view_event/controller.h
        event_operation/view_event/view.h


        event_operation/share/polarizationanalysiscompute.h # test

        event_operation/share/model.h
        event_operation/share/view/graphic_view/graphic_view.h
        event_operation/share/view/graphic_view/wavepick.h
        event_operation/share/view/infoevent.h
        event_operation/share/view/graphiccontroller.h
        event_operation/share/view/chartgesture.h
        event_operation/share/view/eventtoolswidget.h
        event_operation/share/view/graphic_view/pipes2dname.h
        event_operation/share/view/3dscene/polarizationanalysiswindow.h

        event_operation/share/view/polar_graph/polargraph.h # test


        event_operation/share/view/tools_view/wigglewidget.h
        event_operation/share/view/tools_view/hidecomponentwidget.h
        event_operation/share/view/tools_view/clippingwidget.h
        event_operation/share/view/tools_view/gainwidget.h


        horizon_operation/controller.h
        horizon_operation/model.h
        horizon_operation/view/view.h
        horizon_operation/view/addhorizonmanager.h
        horizon_operation/view/filemanager.h


        receiver_operation/controller.h
        receiver_operation/model.h
        receiver_operation/CSVparser.hpp
        receiver_operation/view/view.h
        receiver_operation/view/totalchannelcounter.h
        receiver_operation/view/inforeceiver.h
        receiver_operation/view/addreceivermanager.h


        well_operation/controller.h
        well_operation/model.h
        well_operation/view/view.h
        well_operation/view/addwellmanager.h
        well_operation/view/filemanager.h

        data/seismpolarizationanalysisdata.h
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
        data/io/horizonreadergrd.h


        undo_stack_work/customindividualundostack.h #Note: где должен лежать?
        undo_stack_work/customindividualundocommand.h
        undo_stack_work/shareundocommand.h
        undo_stack_work/event_modification/undocommandgetter.h
        undo_stack_work/event_modification/testindividualcommand.h


        main/controller.h
        main/view/view.h
        main/view/startpage.h
        main/view/workpage.h
        main/view/infoproject.h

#        main/surface_view/surface.h

        main/oil_field_scene/oilfieldscene.h
        main/oil_field_scene/oilfieldwidget.h

        share_view/tableassistant.h
        share_view/filteringtableassistant.h
        share_view/parsing/evaluateExpr.h
        share_view/parsing/parser.h

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

target_link_libraries(qseislab Eigen3::Eigen)
