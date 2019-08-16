#pragma once

#include "data/seismevent.h"
#include "view/view.h"

#include <QObject>

#include <memory>

namespace EventOperation {
namespace AddEvent {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

signals:
  void sendEvent(std::unique_ptr<Data::SeismEvent> &) const;
  void finished() const;

private slots:
  void recvFilePath(const QString &);
  void recvNotification(const QString &);
  void finish(int);

private:
  Model *_model;

  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismEvent> _event;
};

} // namespace AddEvent
} // namespace EventOperation
