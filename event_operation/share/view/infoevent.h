#pragma once

#include <QDateTimeEdit>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>

#include <memory>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
class InfoEvent : public QFrame {
  Q_OBJECT

public:
  explicit InfoEvent(QWidget *parent = nullptr);

  void setBrush(const QBrush &);

  void setDisabled(bool);
  void setEnabled(bool);

  void update(Data::SeismEvent const *const);
  void clear();

  void settingEventInfo(Data::SeismEvent *const) const;

signals:
  void changed() const;

private:
  QLineEdit *_nameEdit;
  QDateEdit *_stampDateEdit;
  QTimeEdit *_stampTimeEdit;

  QLabel *_receiverAmountLabel;
  QLabel *_pWavePickAmountLabel;
  QLabel *_sWavePickAmountLabel;

  QLabel *_addedDateLabel;
  QLabel *_addedTimeLabel;

  bool _allInfoUpdate{false};
};

} // namespace EventOperation
