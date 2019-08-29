#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
class SeismWell;
} // namespace Data

namespace WellOperation {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void viewWells(const std::unique_ptr<Data::SeismProject> &);
  void finish(int);

signals:
  void sendWell(std::unique_ptr<Data::SeismWell> &) const;
  void sendRemovedWell(const QUuid &) const;
  void finished() const;

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismWell> _tmpWell;

  std::vector<std::unique_ptr<Data::SeismWell>> _newWells;
  std::vector<QUuid> _removedWells;
};

} // namespace WellOperation
