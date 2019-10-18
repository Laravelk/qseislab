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
  int _receiverIndex;
  int _waveTypeIndex;
};
} // namespace EventOperation
