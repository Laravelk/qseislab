#pragma once

#include "addreceivermanager.h"

#include <QDialog>
#include <QLabel>
#include <QTableWidget>

#include <memory>

namespace ReceiverOperation {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void addReceiver(const std::unique_ptr<Data::SeismReceiver> &);
  void removeReceiver(const QUuid &);

  void settingReceiverInfo(const std::unique_ptr<Data::SeismReceiver> &);

  void changed(bool);

  void setNotification(const QString &);

signals:
  void sendCsvFilePath(const QString &) const;
  void addReceiverClicked() const;
  void removeReceiverClicked(const QUuid) const;

private slots:
  void handleFromCsvClicked();
  void handleAddReceiverClicked();
  void handleReceiverClicked(int, int);

  void finishReceiverManager(int);

private:
  static const int remove_receiver_col;

  void initReceiversTable(QTableWidget *);
  void insertReceiverInTable(const std::unique_ptr<Data::SeismReceiver> &);

  QLabel *_totalChannelNumLabel;
  QTableWidget *_receiversTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddReceiverManager> _addReceiverManager;
};

} // namespace ReceiverOperation
