#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

#include <memory>

namespace Data {
class SeismHorizon;
}

namespace HorizonOperation {
class FileManager;
class AddHorizonManager : public QDialog {
  Q_OBJECT

public:
  explicit AddHorizonManager(QWidget *parent = nullptr);

  void update(Data::SeismHorizon const * const);

  void settingHorizonInfo(Data::SeismHorizon * const);

signals:
  void sendFilePath(const QString &) const;
  void notify(const QString &) const;

private:
  FileManager *_fileManager;
  QLineEdit *_nameLineEdit;
  QLineEdit *_NxLineEdit;
  QLineEdit *_NyLineEdit;
  QLabel *_pointAmountLabel;
  QPushButton *_addButton;
};

} // namespace HorizonOperation
