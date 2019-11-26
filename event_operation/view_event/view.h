#pragma once

#include "data/seismevent.h"
#include "data/seismwavepick.h"

#include <QWidget>

#include <memory>
#include <set>

namespace Data {
// class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class InfoEvent;
class GraphicController;
class ChartGesture;
namespace ViewEvent {
class View : public QWidget {
  Q_OBJECT

public:
  explicit View(const std::set<QString> &, Data::SeismEvent const *const,
                QWidget *parent = nullptr);

  //  void update(Data::SeismEvent const *const);
  bool allValid() const;

  void updateInfoEvent(Data::SeismEvent const *const);
  void updateDataEvent(Data::SeismEvent const *const);

  void settingEventInfo(Data::SeismEvent *const) const;
  ChartGesture *getChartGesture();

signals:
  void infoChanged() const;

  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindow();

  void finished() const;

  // tool-signals
  //  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

private:
  void updateRepetition(const QString &);

  InfoEvent *_infoEvent;

  GraphicController *_graphicEvent;

  std::map<QUuid, QString> _wellNames_map;
  std::set<QString> _eventNames;
  bool _isValid;
};

} // namespace ViewEvent
} // namespace EventOperation
