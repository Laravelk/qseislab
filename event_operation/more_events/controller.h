#pragma once

#include "view/view.h"

#include <QObject>

#include <QUuid>
#include <map>
#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class Model;
class PolarizationAnalysisWindow;
namespace MoreEvents {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
      const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &,
      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  void sendEvents(std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &) const;
  void finished() const;

private:
  Model *_model;

  std::unique_ptr<View> _view;

  std::map<QUuid, std::unique_ptr<Data::SeismEvent>> _events_map;

  QUuid _currentEventUuid;

  PolarizationAnalysisWindow *_polarizationWindow;
};

} // namespace MoreEvents
} // namespace EventOperation
