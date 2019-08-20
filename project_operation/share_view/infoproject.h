#pragma once

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

  void update(const std::unique_ptr<Data::SeismProject> &);

  void settingProjectInfo(const std::unique_ptr<Data::SeismProject> &);

  void clear();

private:
  QLineEdit *_nameLineEdit;
  QLineEdit *_dateLineEdit;
  QLineEdit *_timeLineEdit;

  // NOTE: можно в приватном поле хранить форматы даты и времени
};

} // namespace ProjectOperation
