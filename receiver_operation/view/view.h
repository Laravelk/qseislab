#pragma once

#include <QDialog>

#include <memory>

class TableAssistant;

class QLabel;

namespace Data {
class SeismReceiver;
}

namespace ReceiverOperation {
class TotalChannelCounter;
class AddReceiverManager;
class View : public QDialog {
  Q_OBJECT

public:
  // explicit View(const std::map<QUuid, QString> &, QWidget *parent = nullptr);
  explicit View(QWidget *parent = nullptr);

  void setWellNames(const std::map<QUuid, QString> &);

  void addReceiver(Data::SeismReceiver const * const);
  void updateReceiver(Data::SeismReceiver const * const);
  void removeReceiver(const QUuid &);

  void viewFullInfo(Data::SeismReceiver const * const);

  const QUuid settingReceiverInfo(Data::SeismReceiver const * const);

  // void changed(bool b = true);

  void setNotification(const QString &);

signals:
  void receiverClicked(const QUuid &) const;
  void sendCsvFilePath(const QString &) const;
  void addReceiverClicked() const;
  void removeReceiverClicked(const QUuid &) const;
  void removeAllReceiverClicked() const;

private slots:
  void handleFromCsvClicked();
  void handleAddReceiverClicked();
  //  void handleReceiverClicked(int, int);

  void finishReceiverManager(int);

private:
  void isChanged(bool);

  TotalChannelCounter *_channelCounter;
  TableAssistant *_receiversTable;
  QPushButton *_saveButton;

  std::unique_ptr<AddReceiverManager> _addReceiverManager;
  std::map<QUuid, QString> _wellNames_map;
};

} // namespace ReceiverOperation
