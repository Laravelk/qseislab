#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismWell;
} // namespace Data

namespace WellOperation {
class Model;
class View;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void viewWells(const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);

  void finish(int);

signals:
  void sendWells(std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);
  void finished() const;

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::shared_ptr<Data::SeismWell> _tmpWell;

  std::map<QUuid, std::shared_ptr<Data::SeismWell>> _wells_map;
};

} // namespace WellOperation
