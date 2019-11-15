#pragma once

#include "addwellmanager.h"

#include <QDialog>

#include <memory>

class TableAssistant;

namespace Data {
class SeismWell;
}

namespace WellOperation {
class AddWellManager;
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void updateWell(Data::SeismWell const *const);
  void addWell(Data::SeismWell const *const);
  void removeWell(const QUuid &);

  void settingWellInfo(Data::SeismWell *const);

  // void changed(bool);

  void setNotification(const QString &);

signals:
  void addWellClicked();
  void sendFilePath(const QString &);
  void removeWellClicked(const QUuid &) const;

private slots:
  void handleAddWellClicked();

private:
  void isChanged(bool);

  TableAssistant *_wellsTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddWellManager> _addWellManager;
};

} // namespace WellOperation
