#pragma once

#include <QDialog>

#include <memory>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
class InfoEvent;
class GraphicEvent;
namespace AddEvent {
class FileManager;
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismEvent> &);
  void setNotification(const QString &);

signals:
  void sendFilePath(const QString &) const;
  void clear() const;

private slots:
  void recvFilePath(const QString &);

private:
  FileManager *_fileManager;
  InfoEvent *_infoEvent;
  GraphicEvent *_graphicEvent;
  QPushButton *_addButton;
  QPushButton *_cancelButton;
};

} // namespace AddEvent
} // namespace EventOperation
