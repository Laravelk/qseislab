#pragma once

#include "data/seismwavepick.h"

#include <QDialog>

#include <QBoxLayout>
#include <memory>
#include <QSlider>
#include <QLabel>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class InfoEvent;
class GraphicController;
class ChartGesture;
namespace Generic {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::unique_ptr<Data::SeismEvent> &,
                QWidget *parent = nullptr);
  explicit View(const std::map<QUuid, QString> &, QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismEvent> &, const QUuid &);
  void update(const std::unique_ptr<Data::SeismEvent> &, const QUuid &,
              const QString &);
  void setNotification(const QString &);
  void settingEventInfo(const std::unique_ptr<Data::SeismEvent> &) const;
  ChartGesture *getChartGesture();

signals:
  void sendWellUuidAndFilePath(const QPair<QUuid, QString> &) const;
  void sendWellUuidForRemove(const QUuid &) const;
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindow();

private:
  void commonSetting();
  void settingGraphicMenu();
  void showGraphicMenu();

  InfoEvent *_infoEvent;
  QVBoxLayout *_wellManagersLayout;
  QPushButton *_addButtonManagers;
  GraphicController *_graphicEvent;
  QPushButton *_okButton;
  QPushButton *_cancelButton;
  QPushButton *_addWaveButton;
  QPushButton *_polarizationEventButton;
  QSlider * _clippingSlider;
  QSlider * _gainSlider;
  QLabel *_clippintSliderLabel;
  QLabel * _gainSliderLabel;

  QAction *_addPWave;
  QAction *_addSWave;

  std::map<QUuid, QString> _wellNames_map;
};

} // namespace Generic
} // namespace EventOperation
