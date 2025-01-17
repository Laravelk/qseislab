#pragma once

#include "data/seismproject.h"

#include <QFileInfo>
#include <QMessageBox>
#include <QObject>

#include <memory>

namespace ProjectOperation {
namespace SaveProject {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void saveProject(std::unique_ptr<Data::SeismProject>);
  void saveAsProject(std::unique_ptr<Data::SeismProject>);

  std::unique_ptr<Data::SeismProject> getProject();

  void finish(int);

signals:
  void finished(bool) const;

private slots:
  void recvFilePath(const QString &);

private:
  bool save(const QFileInfo &);
  static void setNotification(const QString &,
                              QMessageBox::Icon icon = QMessageBox::Critical);

  std::unique_ptr<Data::SeismProject> _project;
};

} // namespace SaveProject
} // namespace ProjectOperation
