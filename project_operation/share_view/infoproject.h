#pragma once

#include <QDateTimeEdit>
#include <QFrame>
#include <QLineEdit>

#include <memory>

namespace Data {
class SeismProject;
}

namespace ProjectOperation {
class InfoProject : public QFrame {
public:
  typedef enum { CLEAN, DEFAULT } MODE;

  explicit InfoProject(MODE, QWidget *parent = nullptr);

  void setDisabled(bool);
  void setEnabled(bool);

  void update(Data::SeismProject const *const);

  void settingProjectInfo(Data::SeismProject *const);

  void clear();

private:
  QLineEdit *_nameLineEdit;
  QDateEdit *_dateEdit;
  QTimeEdit *_timeEdit;

  // NOTE: можно в приватном поле хранить форматы даты и времени
};

} // namespace ProjectOperation
