#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

#include <memory>

namespace Data {
class SeismWell;
}

namespace WellOperation {
class FileManager;
class AddWellManager : public QDialog {
  Q_OBJECT

public:
  explicit AddWellManager(QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismWell> &);

  void settingWellInfo(const std::unique_ptr<Data::SeismWell> &);

signals:
  void sendFilePath(const QString &) const;
  void notify(const QString &) const;

private:
  FileManager *_fileManager;
  QLineEdit *_nameLineEdit;
  QLabel *_pointNumberLabel;
  QPushButton *_addButton;
};

} // namespace WellOperation
