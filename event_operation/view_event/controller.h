#pragma once

#include "view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
namespace ViewEvent {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void viewEvent(const std::unique_ptr<Data::SeismEvent> &);
  void finish(int);

signals:
  void sendEvent(std::unique_ptr<Data::SeismEvent> &);
  void finished() const;

private:
  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismEvent> _event;
};

} // namespace ViewEvent
} // namespace EventOperation
