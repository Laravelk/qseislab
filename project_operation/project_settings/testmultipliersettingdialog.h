#pragma once

#include "settingdialog.h"

#include <QLineEdit>

namespace ProjectOperation {
class TestMultiplierSettingDialog : public SettingDialog {
  Q_OBJECT
public:
  explicit TestMultiplierSettingDialog(QWidget *parent = nullptr);

  void update(const Data::ProjectSettings &) override;

  void setSettings(Data::ProjectSettings &) override;

private:
  QLineEdit *_multEdit;
};
} // namespace ProjectOperation
