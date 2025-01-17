#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismWell;
} // namespace Data

namespace ReceiverOperation {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &,
                      QObject *parent = nullptr);

  void viewReceivers();
  void finish(int);

signals:
  void finished() const;
  void removeAllReceivers() const;
  void sendReciver(const QUuid &, std::unique_ptr<Data::SeismReceiver> &);

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::map<QUuid, std::unique_ptr<Data::SeismWell>> _wells_map;
};

} // namespace ReceiverOperation
