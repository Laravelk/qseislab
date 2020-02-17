#pragma once

#include "settingdialog.h"

#include <QLineEdit>

namespace ProjectOperation {
class TestMultiplierSettingDialog : public SettingDialog {
  Q_OBJECT
public:
  explicit TestMultiplierSettingDialog(QWidget *parent = nullptr);

  void update(Data::ProjectSettings const *const) override;

  void setSettings(Data::ProjectSettings *const) override;

private:
  QLineEdit *_multEdit;
};
} // namespace ProjectOperation
