#pragma once

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include <Qt3DCore/QComponent>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras>

#include "data/seismwavepick.h"

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
class SeismWavePick;
} // namespace Data

namespace EventOperation {
class AnalysisTools;
class PolarizationAnalysisWindow : public QDialog {
  Q_OBJECT
public:
  PolarizationAnalysisWindow(const std::shared_ptr<Data::SeismEvent> &,
                             QDialog *parent = nullptr);
  void setDefault();
  void loadEvent(const std::shared_ptr<Data::SeismEvent> &);
  void update(const Data::SeismEvent *const);
  void removePick(int numOfReciever, Data::SeismWavePick::Type type);

protected:
private:
  const QString P_WAVE_STRING = "PWAVE";
  const QString S_WAVE_STRING = "SWAVE";
  const QString DEFAULT_WAVE_STRING = "Type Wave...";
  const QString DEFAULT_RECEIVER_STRING = "Type Receiver...";
  Qt3DExtras::Qt3DWindow *_view;
  QWidget *_container;
  Qt3DCore::QEntity *_scene;
  const Data::SeismEvent *_event;
  QString _currentWaveTypeString;
  QString _currentReceiverNumberString;

private:
  void drawArrows();
  void drawTextAxes();
  Qt3DCore::QEntity *drawLine(const QVector3D &, const QVector3D &,
                              const QColor &, Qt3DCore::QEntity *);
  void drawCurve(const Data::SeismTrace *const, const Data::SeismTrace *const,
                 const Data::SeismTrace *const, const QColor &,
                 Qt3DCore::QEntity *, const int, const int, const float);
  void drawTraces(const Data::SeismComponent *const);

  int lastElementNumber(const Data::SeismComponent *const);
  bool verifyTheValidity(const Data::SeismEvent *const);

  void clearScene();

  QList<Qt3DCore::QEntity *> _curves;
  QList<Qt3DCore::QEntity *> _arrows;
  Qt3DCore::QEntity *_eigenVectorLine = nullptr;
  AnalysisTools *_analysisTools = nullptr;
};
} // namespace EventOperation
