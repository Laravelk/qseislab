#include "analysistools.h"

#include "data/seismevent.h"

namespace EventOperation {

AnalysisTools::AnalysisTools(const std::shared_ptr<Data::SeismEvent> &event,
                             QWidget *parent)
    : QFrame(parent), _ok(new QPushButton("Ok")),
      _cancle(new QPushButton("Cancle")), _receiverBox(new QComboBox),
      _waveTypeBox(new QComboBox), _infoWidget(new InfoWidget()),
      _event(event.get()) {
  // layout
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_cancle);
  mainLayout->addWidget(_ok);
  mainLayout->addWidget(_receiverBox);
  mainLayout->addWidget(_waveTypeBox);
  mainLayout->addWidget(_infoWidget);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
  // end layout

  // connection
  connect(_ok, &QPushButton::clicked, [this]() { emit okClicked(); });

  connect(_cancle, &QPushButton::clicked, [this]() { emit cancleClicked(); });

  connect(_receiverBox,
          QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
          [=](const QString string) {
            _currentReceiverNumberString = string;
            changeWaveBox();
            emit updatedBox(_currentReceiverNumberString,
                            _currentWaveTypeString);
          });

  connect(_waveTypeBox,
          QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
          [=](const QString string) {
            _currentWaveTypeString = string;
            emit updatedBox(_currentReceiverNumberString,
                            _currentWaveTypeString);
          });
  // end connection

  loadInformation(event);
}

void AnalysisTools::updateInfo(
    const Data::SeismPolarizationAnalysisData &data) {
  _infoWidget->update(data);
}

void AnalysisTools::clearInfo() { _infoWidget->clear(); }

void AnalysisTools::updateBox() { loadInformation(); }

void AnalysisTools::setReceiverBoxIndex(int index) {
  _receiverBox->setCurrentIndex(index);
}

void AnalysisTools::setWaveTypeBoxIndex(int index) {
  _waveTypeBox->setCurrentIndex(index);
}

void AnalysisTools::removeReceiverByIndex(int index) {
  _receiverBox->removeItem(index);
}

void AnalysisTools::removeWaveTypeByIndex(int index) {
  _waveTypeBox->removeItem(index);
}

const QComboBox &AnalysisTools::getReceiverBox() { return *_receiverBox; }

const QComboBox &AnalysisTools::getWaveBox() { return *_waveTypeBox; }

void AnalysisTools::loadInformation(
    const std::shared_ptr<Data::SeismEvent> &event) {
  _event = event.get();
  loadInformation();
}

void AnalysisTools::loadInformation() {
  QList<QString> waveTypeList;
  QList<QString> receiverList;

  waveTypeList.append(DEFAULT_WAVE_STRING);
  waveTypeList.append(P_WAVE_STRING);
  waveTypeList.append(S_WAVE_STRING);
  _waveTypeBox->addItems(waveTypeList);

  receiverList.append(DEFAULT_RECEIVER_STRING);
  for (int i = 0; i < _event->getComponentAmount(); ++i) {
    receiverList.append(QString::number(i));
  }
  _receiverBox->addItems(receiverList);
}

void AnalysisTools::changeWaveBox() {
  QList<QString> waveTypeList;
  int itemCount = _waveTypeBox->count();
  _waveTypeBox->setCurrentIndex(0);
  for (int i = 0; i < itemCount; i++) {
    _waveTypeBox->removeItem(1);
  }
  if (DEFAULT_RECEIVER_STRING != _currentReceiverNumberString) {
    int index = 0;
    for (auto &component : _event->getComponents()) {
      if (index == _currentReceiverNumberString.toInt()) {
        for (auto &pick : component->getWavePicks()) {
          if (Data::SeismWavePick::PWAVE == pick.first) {
            waveTypeList.append(P_WAVE_STRING);
          }
          if (Data::SeismWavePick::SWAVE == pick.first) {
            waveTypeList.append(S_WAVE_STRING);
          }
        }
      }
      index++;
    }
  } else {
    waveTypeList.append(P_WAVE_STRING);
    waveTypeList.append(S_WAVE_STRING);
  }
  _waveTypeBox->addItems(waveTypeList);
  _waveTypeBox->setCurrentIndex(0);
  _currentWaveTypeString = DEFAULT_WAVE_STRING;
}
}
