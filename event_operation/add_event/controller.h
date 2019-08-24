#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismEvent;
class SeismReciever;
} // namespace Data

namespace EventOperation {
namespace AddEvent {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(const std::list<std::unique_ptr<Data::SeismReciever>> &,
                      QObject *parent = nullptr);

  void start();

signals:
  void sendEvent(std::unique_ptr<Data::SeismEvent> &) const;
  void finished() const;

private slots:
  void recvFilePath(const QString &);
  void recvNotification(const QString &);
  void finish(int);

private:
  const std::list<std::unique_ptr<Data::SeismReciever>> &_recievers;

  Model *_model;

  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismEvent> _event;
};

} // namespace AddEvent
} // namespace EventOperation
