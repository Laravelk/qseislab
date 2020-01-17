#pragma once

#include <QBoxLayout>
#include <QDialog>
#include <QPushButton>

#include <data/seismevent.h>

namespace Data {
class ProjectSettings;
}

namespace ProjectOperation {
class SettingDialog : public QDialog {
  Q_OBJECT
public:
  SettingDialog(QWidget *parent = nullptr);

  virtual void update(Data::ProjectSettings const *const) = 0;

  virtual void setSettings(Data::ProjectSettings *const) = 0;

signals:
  void apply() const;

protected:
  void hasChanged(bool);

  QHBoxLayout *_buttonsLayout;

private:
  QPushButton *_okButton;
  QPushButton *_applyButton;
  QPushButton *_cancelButton;
};

SettingDialog *getSettingDialog(Data::SeismEvent::TransformOperation oper);

} // namespace ProjectOperation
