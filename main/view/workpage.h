#pragma once

#include "data/seismproject.h"

#include <QFrame>
#include <QTableWidget>
#include <QtDataVisualization>
#include <memory>

using namespace QtDataVisualization;

namespace Main {
class InfoProject;
class Surface;
class WorkPage : public QFrame {
    Q_OBJECT

public:
    explicit WorkPage(QWidget* parent = nullptr);

    void loadProject(const std::unique_ptr<Data::SeismProject>& );

    void updateProject(const std::unique_ptr<Data::SeismEvent>& );
    void updateProjectRemoveEvent(const Data::SeismEvent::Uuid& );
    void updateProject(const std::unique_ptr<Data::SeismHorizon>& );
    void updateProjectRemoveHorizon(const Data::SeismHorizon::Uuid& );


signals:
    void addEventClicked() const;
    void viewEventClicked(const Data::SeismEvent::Uuid ) const;
    void removeEventClicked(const Data::SeismEvent::Uuid ) const;

    void addHorizonClicked() const;

    void saveProjectClicked() const;
    void closeProjectClicked() const;

private slots:
    void handleAddEventClicked();
    void handleEventClicked(int, int);

    void handleAddHorizonClicked();

    void handleSaveProjectClicked();
    void handleCloseProjectClicked();

private:
    void clearTable();
    void initEventsTable(QTableWidget* );

    void insertEventInTable(const std::unique_ptr<Data::SeismEvent>& );
    void removeEventInTable(const Data::SeismEvent::Uuid& );

    InfoProject* _infoProject;
    QTableWidget* _eventsTable;
    Surface* _surface;
    Q3DSurface* _graph;
};


} // namespace Main
