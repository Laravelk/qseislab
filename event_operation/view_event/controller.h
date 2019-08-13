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

  void viewEvent(const std::unique_ptr<Data::SeismEvent> &) const;

signals:
  void finished() const;

private slots:
  void finish(int);

private:
  std::unique_ptr<View> _view;
};

} // namespace ViewEvent
} // namespace EventOperation
