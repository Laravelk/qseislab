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
  PolarizationAnalysisWindow(const std::unique_ptr<Data::SeismEvent> &,
                             QDialog *parent = nullptr);

protected:
private:
  QPushButton *_okButton;
  QPushButton *_cancelButton;
  QComboBox *_receiverBox;
  QComboBox *_waveTypeBox;
  Qt3DExtras::Qt3DWindow *_view;
  QWidget *_container;
  Qt3DCore::QEntity *_scene;
  const Data::SeismEvent *_event;
  int _receiverIndex;
  int _waveTypeIndex;

private:
  void drawArrows();
  Qt3DCore::QEntity *drawLine(const QVector3D &, const QVector3D &,
                              const QColor &, Qt3DCore::QEntity *);
  void drawCurve(const std::unique_ptr<Data::SeismTrace> &,
                 const std::unique_ptr<Data::SeismTrace> &,
                 const std::unique_ptr<Data::SeismTrace> &, const QColor &,
                 Qt3DCore::QEntity *, const int, const int, const float);
  void drawTraces(const std::unique_ptr<Data::SeismComponent> &);
  int lastElementNumber(const std::unique_ptr<Data::SeismComponent> &);
  void update();
  Qt3DCore::QEntity *createTestScene();
  void clearScene();
  QList<Qt3DCore::QEntity *> _curves;
  QList<Qt3DCore::QEntity *> _arrows;
};
} // namespace EventOperation
