#pragma once

#include <QBoxLayout>
#include <QDialog>
#include <QPushButton>

namespace Data {
class ProjectSettings;
}

namespace ProjectOperation {
class SettingDialog : public QDialog {
  Q_OBJECT
public:
  SettingDialog(QWidget *parent = nullptr);

  virtual void update(const Data::ProjectSettings &) = 0;

  virtual void setSettings(Data::ProjectSettings &) = 0;

signals:
  void apply() const;

protected:
  void hasChanged();

  QHBoxLayout *_buttonsLayout;

private:
  QPushButton *_okButton;
  QPushButton *_applyButton;
  QPushButton *_cancelButton;
};
} // namespace ProjectOperation
