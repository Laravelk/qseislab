#pragma once

#include "addreceivermanager.h"
#include "share_view/tableassistant.h"

#include <QDialog>
#include <QLabel>

#include <memory>

namespace Data {
class SeismReceiver;
}

namespace ReceiverOperation {
class TotalChannelCounter;
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::map<QUuid, QString> &, QWidget *parent = nullptr);

  void addReceiver(const std::unique_ptr<Data::SeismReceiver> &);
  void removeReceiver(const QUuid &);

  void viewFullInfo(const std::unique_ptr<Data::SeismReceiver> &);

  const QUuid settingReceiverInfo(const std::unique_ptr<Data::SeismReceiver> &);

  void changed(bool b = true);

  void setNotification(const QString &);

signals:
  void receiverClicked(const QUuid &) const;
  void sendCsvFilePath(const QString &) const;
  void addReceiverClicked() const;
  void removeReceiverClicked(const QUuid &) const;

private slots:
  void handleFromCsvClicked();
  void handleAddReceiverClicked();
  //  void handleReceiverClicked(int, int);

  void finishReceiverManager(int);

private:
  //  QLabel *_totalChannelNumLabel;
  TotalChannelCounter *_channelCounter;
  TableAssistant *_receiversTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddReceiverManager> _addReceiverManager;
  std::map<QUuid, QString> _wellNames_map;
};

} // namespace ReceiverOperation
