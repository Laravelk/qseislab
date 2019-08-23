#pragma once

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

  void setDisabled(bool);
  void setEnabled(bool);

  void update(const std::unique_ptr<Data::SeismEvent> &);
  void clear();

  void settingEventInfo(const std::unique_ptr<Data::SeismEvent> &) const;

private:
  QLineEdit *_dateLineEdit;
  QLineEdit *_timeLineEdit;
  QLabel *_traceNumberLabel;
  QLabel *_lengthLabel;
  QLabel *_groupeCoordinate;
};

} // namespace EventOperation