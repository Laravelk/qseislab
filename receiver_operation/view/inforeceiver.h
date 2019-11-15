#pragma once

#include <QDialog>

#include <memory>

class QLineEdit;
class QTableWidget; // TODO: переделать на нормальную таблицу (TableAssistant)!

namespace Data {
class SeismReceiver;
class SeismChannelReceiver;
} // namespace Data

namespace ReceiverOperation {
class InfoReceiver : public QDialog {
  Q_OBJECT
public:
  InfoReceiver(Data::SeismReceiver const * const, QWidget *parent = nullptr);

private:
  void initChannelssTable(QTableWidget *);
  void
  insertChannelInTable(const std::unique_ptr<Data::SeismChannelReceiver> &);

  QLineEdit *_nameLineEdit;
  QTableWidget *_channelsTable;
};
} // namespace ReceiverOperation
