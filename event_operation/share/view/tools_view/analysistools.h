#pragma once

#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>

#include "infowidget.h"

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
}

namespace EventOperation {
class AnalysisTools : public QFrame {
  Q_OBJECT
public:
  AnalysisTools(const std::shared_ptr<Data::SeismEvent> &,
                QWidget *parent = nullptr);

  void updateInfo(const Data::SeismPolarizationAnalysisData &);
  void clearInfo();

  void updateBox();

  void setReceiverBoxIndex(int);
  void setWaveTypeBoxIndex(int);

  void removeReceiverByIndex(int);
  void removeWaveTypeByIndex(int);

  void loadInformation(const std::shared_ptr<Data::SeismEvent> &);

  const QComboBox &getReceiverBox();
  const QComboBox &getWaveBox();

  const QString P_WAVE_STRING = "PWAVE";
  const QString S_WAVE_STRING = "SWAVE";
  const QString DEFAULT_WAVE_STRING = "Type Wave...";
  const QString DEFAULT_RECEIVER_STRING = "Type Receiver...";

signals:
  void okClicked();
  void cancleClicked();
  void receiverIndexChanged(QString receiver);
  void waveIndexChanged(QString wave);
  void updatedBox(QString currentReceiver, QString currentWave);

private:
  QPushButton *_ok;
  QPushButton *_cancle;
  QComboBox *_receiverBox;
  QComboBox *_waveTypeBox;
  InfoWidget *_infoWidget;

  QString _currentWaveTypeString;
  QString _currentReceiverNumberString;

  Data::SeismEvent *_event;

  void changeWaveBox();
  void loadInformation();
};
}
