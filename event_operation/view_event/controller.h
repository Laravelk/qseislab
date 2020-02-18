#pragma once

#include "data/seismevent.h"

#include <QObject>

#include <memory>

class QUndoStack;

namespace Data {
class SeismWell;
class ProjectSettings;
} // namespace Data

namespace EventOperation {
class View;
class PolarizationAnalysisWindow;
class PolarizationAnalysisCompute;
namespace ViewEvent {
class View;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
      const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
      Data::ProjectSettings *const, const std::shared_ptr<Data::SeismEvent> &,
      QObject *parent = nullptr);

  QWidget *getView();

  // TODO: re-see!
  void f1();
  void f2(Data::SeismEvent const *const);
  void f3(Data::SeismEvent const *const);
  virtual ~Controller();
  // ...

signals:
  void undoClicked(const QUuid &) const;
  void redoClicked(const QUuid &) const;
  void eventActionClicked(const QUuid &,
                          Data::SeismEvent::TransformOperation) const;

  void finished() const;

private:
  View *_view;

  std::map<QUuid, QString> _eventNameContainer;

  PolarizationAnalysisWindow *_polarizationWindow;
  PolarizationAnalysisCompute *_calculatePolarization = nullptr;


  std::shared_ptr<Data::SeismEvent> _event;
};

} // namespace ViewEvent
} // namespace EventOperation
