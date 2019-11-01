#pragma once

#include "addwellmanager.h"
#include "share_view/tableassistant.h"

#include <QDialog>

#include <memory>

namespace Data {
class SeismWell;
}

namespace WellOperation {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void updateWell(const std::unique_ptr<Data::SeismWell> &);
  void addWell(const std::unique_ptr<Data::SeismWell> &);
  void removeWell(const QUuid &);

  void settingWellInfo(const std::unique_ptr<Data::SeismWell> &);

  void changed(bool);

  void setNotification(const QString &);

signals:
  void addWellClicked();
  void sendFilePath(const QString &);
  void removeWellClicked(const QUuid &) const;

private slots:
  void handleAddWellClicked();

private:
  TableAssistant *_wellsTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddWellManager> _addWellManager;
};

} // namespace WellOperation
