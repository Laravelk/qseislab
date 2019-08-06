#pragma once

#include <QFrame>
#include <QTableWidget>

#include <memory>


namespace Data {
    class SeismProject;
    class SeismEvent;
}

namespace Main {
class InfoProject;
class WorkPage : public QFrame {
    Q_OBJECT

public:
    explicit WorkPage(QWidget* parent = nullptr);

    void updateProject(const std::unique_ptr<Data::SeismProject>& );

signals:
    void addEventClicked() const;
    void addHorizonClicked() const;
    void saveProjectClicked() const;
    void closeProjectClicked() const;

private slots:
    void handleAddEventClicked();
    void handleAddHorizonClicked();
    void handleSaveProjectClicked();
    void handleCloseProjectClicked();

private:
    void clearTable();
    void initEventsTable(QTableWidget* );
    void insertEventsInTable(const std::unique_ptr<Data::SeismProject>& );

    InfoProject* _infoProject;
    QTableWidget* _eventsTable;
};


} // namespace Main
