#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class Model;
class PolarizationAnalysisWindow;
namespace OneEvent {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &,
                      QObject *parent = nullptr);

  explicit Controller(const std::unique_ptr<Data::SeismEvent> &,
                      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  void sendEvent(std::unique_ptr<Data::SeismEvent> &) const;
  void finished() const;

private:
  QString generateEventName() const;
  Model *_model;

  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismEvent> _event;
  std::map<QUuid, QString> _eventNameContainer;

  PolarizationAnalysisWindow *_polarizationWindow;
};

} // namespace OneEvent
} // namespace EventOperation
