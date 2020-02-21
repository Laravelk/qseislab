#pragma once

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QPushButton>

#include <Qt3DCore/QComponent>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

namespace EventOperation {
class PolarizationAnalysisWindow : public QDialog {
  Q_OBJECT
public:
  PolarizationAnalysisWindow(const std::shared_ptr<Data::SeismEvent> &,
                             QDialog *parent = nullptr);
  void setDefault();

protected:
private:
  const QString P_WAVE_STRING = "PWAVE";
  const QString S_WAVE_STRING = "SWAVE";
  const QString DEFAULT_WAVE_STRING = "Type Wave...";
  const QString DEFAULT_RECEIVER_STRING = "Type Receiver...";
  QPushButton *_okButton;
  QPushButton *_cancelButton;
  QComboBox *_receiverBox;
  QComboBox *_waveTypeBox;
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
  void update();
  void changeWaveBox();
  void changeReceiverNumberBox();
  Qt3DCore::QEntity *createTestScene();
  void clearScene();
  QList<Qt3DCore::QEntity *> _curves;
  QList<Qt3DCore::QEntity *> _arrows;
  Qt3DCore::QEntity * _eigenVectorLine = nullptr;
};
} // namespace EventOperation
