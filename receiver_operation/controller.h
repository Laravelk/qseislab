#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismWell;
class SeismReceiver;
} // namespace Data

namespace ReceiverOperation {
class Model;
class View;
class Controller : public QObject {
  Q_OBJECT

public:
  // explicit Controller(const std::map<QUuid, std::unique_ptr<Data::SeismWell>>
  // &,
  //                     QObject *parent = nullptr);

  explicit Controller(QObject *parent = nullptr);

  void viewReceivers(const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);
  void finish(int);

signals:
  void finished() const;
  void removeAllReceivers() const;
  void sendReciver(const QUuid &, const std::shared_ptr<Data::SeismReceiver> &);

  void sendWells(std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::map<QUuid, std::shared_ptr<Data::SeismWell>> _wells_map;
};

} // namespace ReceiverOperation
