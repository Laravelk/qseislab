#pragma once

#include "addhorizonmanager.h"

#include <QDialog>
#include <QTableWidget>

#include <memory>

namespace Data {
class SeismHorizon;
}

namespace HorizonOperation {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void updateHorizon(const std::unique_ptr<Data::SeismHorizon> &);
  void addHorizon(const std::unique_ptr<Data::SeismHorizon> &);
  void removeHorizon(const QUuid &);

  void settingHorizonInfo(const std::unique_ptr<Data::SeismHorizon> &);

  void changed(bool);

public slots:
  void setNotification(const QString &);

signals:
  void addHorizonClicked();
  void sendFilePath(const QString &);
  void removeHorizonClicked(const QUuid) const;

private slots:
  void handleAddHorizonClicked();
  void handleHorizonClicked(int, int);

  void recvFilePath(const QString &);
  void finishHorizonManager(int);

private:
  void initHorizonsTable(QTableWidget *);
  void insertHorizonInTable(const std::unique_ptr<Data::SeismHorizon> &);

  QTableWidget *_horizonsTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddHorizonManager> _addHorizonManager;
};

} // namespace HorizonOperation
