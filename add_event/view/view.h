#pragma once

#include <QDialog>
#include <QPushButton>
#include <QScrollArea>

#include <memory>


namespace Data {
    class SeismEvent;
}

namespace AddEvent {
class FileManager;
class InfoEvent;
class SimulationGraphic; // need to remove
class GraphicEvent;
class View : public QDialog {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);

    void update(const std::unique_ptr<Data::SeismEvent>& );
    void setNotification(const QString& );

signals:
    void sendFilePath(const QString& ) const;
    void clear() const;

private slots:
    void recvFilePath(const QString& );

private:
    FileManager* _fileManager;
    InfoEvent* _infoEvent;
    SimulationGraphic* _simulationGraphic; // need to remove
    QScrollArea *_scrollArea;

    GraphicEvent* _graphicEvent;
    QPushButton* _addButton;
    QPushButton* _cancelButton;
};

} // namespace AddEvent
