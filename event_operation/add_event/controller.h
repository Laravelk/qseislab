#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
namespace AddEvent {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &,
                      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  void sendEvent(std::unique_ptr<Data::SeismEvent> &) const;
  void finished() const;

private:
  const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &_wells_map;

  Model *_model;

  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismEvent> _event;
};

} // namespace AddEvent
} // namespace EventOperation
