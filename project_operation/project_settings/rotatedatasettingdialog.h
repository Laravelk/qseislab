#pragma once

#include "settingdialog.h"

#include <QRadioButton>

namespace ProjectOperation {
class RotateDataSettingDialog : public SettingDialog {
  Q_OBJECT
public:
  explicit RotateDataSettingDialog(QWidget *parent = nullptr);

  void update(Data::ProjectSettings const *const) override;

  void setSettings(Data::ProjectSettings *const) override;

private:
  QRadioButton *_toEBasisButton;
  QRadioButton *_toReceiverBasisButton;
};

} // namespace ProjectOperation
