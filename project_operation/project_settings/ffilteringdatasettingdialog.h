#pragma once

#include "settingdialog.h"

#include <QLineEdit>

namespace ProjectOperation {
class FFilteringDataSettingDialog : public SettingDialog {
  Q_OBJECT
public:
  explicit FFilteringDataSettingDialog(QWidget *parent = nullptr);

  void update(const Data::ProjectSettings &) override;

  void setSettings(Data::ProjectSettings &) override;

private:
  QLineEdit *_f1Edit;
  QLineEdit *_f2Edit;
  QLineEdit *_f3Edit;
  QLineEdit *_f4Edit;
};
} // namespace ProjectOperation
