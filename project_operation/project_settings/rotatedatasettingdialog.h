#pragma once

#include "settingdialog.h"

#include <QRadioButton>

namespace ProjectOperation {
class RotateDataSettingDialog : public SettingDialog {
  Q_OBJECT
public:
  explicit RotateDataSettingDialog(QWidget *parent = nullptr);

  void update(const Data::ProjectSettings &) override;

  void setSettings(Data::ProjectSettings &) override;

private:
  QRadioButton *_toEBasisButton;
  QRadioButton *_toReceiverBasisButton;
};

} // namespace ProjectOperation
