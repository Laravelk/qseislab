#pragma once

#include <QMainWindow>
#include <QMenuBar>

#include <memory>


namespace Data {
    class SeismEvent;
    class SeismProject;
}

namespace Main {
class WorkPage;
class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget* parent = nullptr);

    void loadProject(const std::unique_ptr<Data::SeismProject>& );
    void updateProject(const std::unique_ptr<Data::SeismProject>& );
    void closeProject();

signals:
    void addEventClicked() const;
    void addHorizonClicked() const;
    void newProjectClicked() const;
    void openProjectClicked() const;
    void saveProjectClicked() const;
    void closeProjectClicked() const;

private slots:
    void handleAddEventClicked();
    void handleAddHorizonClicked();
    void handleNewProjectClicked();
    void handleOpenProjectClicked();
    void handleSaveProjectClicked();
    void handleCloseProjectClicked();

private:
    WorkPage* _workPage{nullptr}; // NOTE: правильно ли хранить этот указатель - ?
};


} // namespace Main
