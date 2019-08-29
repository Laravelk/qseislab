#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismEvent;
class SeismReceiver;
} // namespace Data

namespace EventOperation {
namespace AddEvent {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(const std::list<std::unique_ptr<Data::SeismReceiver>> &,
                      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  void sendEvent(std::unique_ptr<Data::SeismEvent> &) const;
  void finished() const;

private:
  const std::list<std::unique_ptr<Data::SeismReceiver>> &_receivers;

  Model *_model;

  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismEvent> _event;
};

} // namespace AddEvent
} // namespace EventOperation
