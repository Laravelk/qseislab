#pragma once

#include <QDialog>

#include <memory>

namespace Data {
class SeismProject;
}

namespace ProjectOperation {
class FileManager;
class InfoProject;
namespace OpenProject {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismProject> &);
  void setNotification(const QString &);

  void settingProjectInfo(const std::unique_ptr<Data::SeismProject> &);

signals:
  void sendFilePath(const QString &) const;

private slots:
  void recvFilePath(const QString &);

private:
  FileManager *_fileManager;
  InfoProject *_infoProject;
  QPushButton *_openButton;
  QPushButton *_cancelButton;
};

} // namespace OpenProject
} // namespace ProjectOperation
