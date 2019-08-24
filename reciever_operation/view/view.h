#pragma once

#include "addrecievermanager.h"

#include <QDialog>
#include <QLabel>
#include <QTableWidget>

#include <memory>

namespace RecieverOperation {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void addReciever(const std::unique_ptr<Data::SeismReciever> &);
  void removeReciever(const QUuid &);

  void settingRecieverInfo(const std::unique_ptr<Data::SeismReciever> &);

  void changed(bool);

public slots:
  void setNotification(const QString &);

signals:
  void addRecieverClicked();
  void removeRecieverClicked(const QUuid) const;

private slots:
  void handleAddRecieverClicked();
  void handleRecieverClicked(int, int);

  void finishRecieverManager(int);

private:
  void initRecieversTable(QTableWidget *);
  void insertRecieverInTable(const std::unique_ptr<Data::SeismReciever> &);

  QLabel *_totalChannelNumLabel;
  QTableWidget *_recieversTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddRecieverManager> _addRecieverManager;
};

} // namespace RecieverOperation
