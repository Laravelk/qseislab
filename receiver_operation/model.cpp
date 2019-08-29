#include "model.h"

#include "data/seismreceiver.h"

#include <QFile>
#include <QTextStream>
//#include <QxtCsvModel>

typedef Data::SeismReceiver SeismReceiver;

namespace ReceiverOperation {
Model::Model(QObject *parent) : QObject(parent) {}

std::list<std::unique_ptr<SeismReceiver>>
Model::getSeismReceiversFrom(const QString &path) {
  std::list<std::unique_ptr<SeismReceiver>> receiversList;

  QString err_msg;

  QFile file(path);
  if (file.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&file);
    int i = 0;
    while (!in.atEnd()) {
      QString line = in.readLine();
      if (0 == i++)
        continue;

      std::unique_ptr<SeismReceiver> receiver =
          std::make_unique<SeismReceiver>();
      int j = 0;
      float x, y, z;
      float or_x, or_y, or_z;
      for (QString item : line.split(",")) {
        switch (j) {
        case 2:
          receiver->setName(item);
          break;
        case 4:
          x = item.toFloat();
          break;
        case 5:
          y = item.toFloat();
          break;
        case 6:
          z = item.toFloat();
          break;
        case 15:
          or_x = item.toFloat();
          break;
        case 16:
          or_y = item.toFloat();
          break;
        case 17:
          or_z = item.toFloat();
          break;
        }
        ++j;
      }
      receiver->setLocation({x, y, z});
      //      receiver->setOrientation({or_x, or_y, or_z});
      //      receiver->setChannelNum(1);
      receiversList.push_back(std::move(receiver));
    }

  } else {
    err_msg = "File not opened\n";
  }

  emit notify("TODO: csv-reading");

  if (!err_msg.isEmpty()) {
    receiversList.clear();
    emit notify(err_msg);
  }

  return receiversList;
}

} // namespace ReceiverOperation
