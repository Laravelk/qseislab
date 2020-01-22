#pragma once

#include "settingdialog.h"

#include <Eigen/Dense>

#include <QLineEdit>
#include <QRadioButton>

namespace ProjectOperation {
class IndefiniteBasisEditWidget : public QFrame{
    Q_OBJECT
public:
    IndefiniteBasisEditWidget(QWidget* parent = nullptr);

    void setBasis(const Eigen::Matrix3f& );
     Eigen::Matrix3f getBasis() const;

    bool isValid() const;

 signals:
    void changed() const;

private:
    QLineEdit* crds[9];
};

class RotateDataSettingDialog : public SettingDialog {
  Q_OBJECT
public:
  explicit RotateDataSettingDialog(QWidget *parent = nullptr);

  void update(Data::ProjectSettings const *const) override;

  void setSettings(Data::ProjectSettings *const) override;

private:
  QRadioButton *_toEBasisButton;
  QRadioButton *_toReceiverBasisButton;
  QRadioButton *_toIndefiniteBasisButton;
  IndefiniteBasisEditWidget* _indefiniteEditWidget;
};

} // namespace ProjectOperation
