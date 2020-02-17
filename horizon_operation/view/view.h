#pragma once

#include "addhorizonmanager.h"
#include "share_view/tableassistant.h"

#include <QDialog>

#include <memory>

class TableAssistant;

namespace Data {
class SeismHorizon;
}

namespace HorizonOperation {
class AddHorizonManager;
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void addHorizon(Data::SeismHorizon const *const);
  void updateHorizon(Data::SeismHorizon const *const);

  void removeHorizon(const QUuid &);

  void settingHorizonInfo(Data::SeismHorizon *const);

  void setNotification(const QString &);

signals:
  void addHorizonClicked();
  void sendFilePath(const QString &);
  void removeHorizonClicked(const QUuid &) const;

private slots:
  void handleAddHorizonClicked();

private:
  void isChanged(bool);

  TableAssistant *_horizonsTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddHorizonManager> _addHorizonManager;
};

} // namespace HorizonOperation
