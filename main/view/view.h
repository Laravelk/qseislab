#pragma once

#include "data/seismproject.h"

#include <QMainWindow>
#include <QMenuBar>

#include <memory>


namespace Main {
class WorkPage;
class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget* parent = nullptr);

    void loadProject(const std::unique_ptr<Data::SeismProject>& );
    void updateProject(const std::unique_ptr<Data::SeismEvent>& );
    void updateProject(const std::unique_ptr<Data::SeismHorizon>& );
    void updateProjectRemoveEvent(const Data::SeismEvent::Uuid& );
    void updateProjectRemoveHorizon(const Data::SeismHorizon::Uuid& );
    void closeProject();

signals:
    void addEventClicked() const;
    void viewEventClicked(const Data::SeismEvent::Uuid ) const;
    void removeEventClicked(const Data::SeismEvent::Uuid ) const;

    void addHorizonClicked() const;

    void newProjectClicked() const;
    void openProjectClicked() const;
    void saveProjectClicked() const;
    void closeProjectClicked() const;

private slots:
    void handleAddEventClicked();
    void handleViewEventClicked(const Data::SeismEvent::Uuid );
    void handleRemoveEventClicked(const Data::SeismEvent::Uuid );

    void handleAddHorizonClicked();

    void handleNewProjectClicked();
    void handleOpenProjectClicked();
    void handleSaveProjectClicked();
    void handleCloseProjectClicked();

private:
    WorkPage* _workPage{nullptr}; // NOTE: правильно ли хранить этот указатель - ?
};


} // namespace Main
