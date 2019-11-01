#pragma once

#include "addhorizonmanager.h"
#include "share_view/tableassistant.h"

#include <QDialog>

#include <memory>

namespace Data {
class SeismHorizon;
}

namespace HorizonOperation {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void addHorizon(const std::unique_ptr<Data::SeismHorizon> &);
  void updateHorizon(const std::unique_ptr<Data::SeismHorizon> &);
  void removeHorizon(const QUuid &);

  void settingHorizonInfo(const std::unique_ptr<Data::SeismHorizon> &);

  void changed(bool);

  void setNotification(const QString &);

signals:
  void addHorizonClicked();
  void sendFilePath(const QString &);
  void removeHorizonClicked(const QUuid &) const;

private slots:
  void handleAddHorizonClicked();

private:
  TableAssistant *_horizonsTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddHorizonManager> _addHorizonManager;
};

} // namespace HorizonOperation
