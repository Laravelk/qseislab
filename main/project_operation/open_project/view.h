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

  void update(Data::SeismProject const *const);
  void setNotification(const QString &);

  void settingProjectInfo(Data::SeismProject *const);

signals:
  void sendFilePath(const QString &) const;

private:
  FileManager *_fileManager;
  InfoProject *_infoProject;
  QPushButton *_openButton;
  QPushButton *_cancelButton;
};

} // namespace OpenProject
} // namespace ProjectOperation
