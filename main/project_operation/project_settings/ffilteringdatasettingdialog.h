#pragma once

#include "settingdialog.h"

#include <QLineEdit>

namespace ProjectOperation {
class FFilteringDataSettingDialog : public SettingDialog {
  Q_OBJECT
public:
  explicit FFilteringDataSettingDialog(QWidget *parent = nullptr);

  void update(Data::ProjectSettings const *const) override;

  void setSettings(Data::ProjectSettings *const) override;

private slots:
  void inputChange();

private:
  QLineEdit *_f1Edit;
  QLineEdit *_f2Edit;
  QLineEdit *_f3Edit;
  QLineEdit *_f4Edit;
};
} // namespace ProjectOperation
