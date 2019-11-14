#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QTableWidget>

#include <memory>

namespace Data {
class SeismReceiver;
class SeismChannelReceiver;
} // namespace Data

namespace ReceiverOperation {
class InfoReceiver : public QDialog {
  Q_OBJECT
public:
  InfoReceiver(const std::shared_ptr<Data::SeismReceiver> &,
               QWidget *parent = nullptr);

private:
  void initChannelssTable(QTableWidget *);
  void
  insertChannelInTable(const std::shared_ptr<Data::SeismChannelReceiver> &);

  QLineEdit *_nameLineEdit;
  QTableWidget *_channelsTable;
};
} // namespace ReceiverOperation
