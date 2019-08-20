#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
class SeismHorizon;
} // namespace Data

namespace HorizonOperation {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void viewHorizons(const std::unique_ptr<Data::SeismProject> &);

signals:
  void sendHorizon(std::unique_ptr<Data::SeismHorizon> &);
  void sendRemovedHorizon(const QUuid &);
  void finished() const;

private slots:
  void handleRemoveHorizonClicked(const QUuid);
  void handleAddHorizonClicked();
  void recvFilePath(const QString &);
  void recvNotification(const QString &);
  void finish(int);

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismHorizon> _tmpHorizon;

  std::vector<std::unique_ptr<Data::SeismHorizon>> _newHorizons;
  std::vector<QUuid> _removedHorizons;
};

} // namespace HorizonOperation
