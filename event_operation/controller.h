#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class PolarizationAnalysisWindow;
namespace Generic {
class Model;
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
  Model *_model;

  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismEvent> _event;

  PolarizationAnalysisWindow *_polarizationWindow;
};

} // namespace Generic
} // namespace EventOperation
