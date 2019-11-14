#pragma once

#include <QDialog>
// #include <QLabel>
// #include <QLineEdit>
#include <memory>

class QLabel;
class QLineEdit;


namespace Data {
class SeismWell;
}

namespace WellOperation {
class FileManager;
class AddWellManager : public QDialog {
  Q_OBJECT

public:
  explicit AddWellManager(QWidget *parent = nullptr);

  void update(Data::SeismWell const * const);

  void settingWellInfo(Data::SeismWell * const);

signals:
  void sendFilePath(const QString &) const;
  void notify(const QString &) const;

private:
  FileManager *_fileManager;
  QLineEdit *_nameLineEdit;
  QLabel *_pointAmountLabel;
  QPushButton *_addButton;
};

} // namespace WellOperation
