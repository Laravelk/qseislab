#include "seismwell.h"

#include "data/io/pointreader.h"
#include "data/io/pointwriter.h"

#include <QJsonArray>

#include <assert.h>

typedef Data::IO::PointReader PointReader;
typedef Data::IO::PointWriter PointWriter;

namespace Data {
const QString SeismWell::_default_path = "data/wells/";

SeismWell::SeismWell() : _uuid(QUuid::createUuid()) {}

SeismWell::SeismWell(const QJsonObject &json, const QDir &dir)
//    : _uuid(QUuid::createUuid())
{

  std::string err_msg;

  if (json.contains("uuid")) {
    _uuid = QUuid::fromString(json["uuid"].toString());
  } else {
    err_msg += "::uuid : not found\n";
  }

  if (json.contains("name")) {
    _name = json["name"].toString();
  } else {
    err_msg += "::name : not found\n";
  }

  if (json.contains("path")) {
    _path = json["path"].toString();
  } else {
    err_msg += "::path : not found\n";
  }

  //  if (json.contains("Receivers")) {
  //    QJsonArray receiversArray(json["Receivers"].toArray());
  //    int idx = 0;
  //    for (auto objReceiver : receiversArray) {
  //      try {
  //        auto seismReceiver =
  //            std::make_shared<SeismReceiver>(objReceiver.toObject());
  //        _receivers.push_back(std::move(seismReceiver));
  //      } catch (std::runtime_error &err) {
  //        err_msg += "Receivers (idx: " + std::to_string(idx) + ")\n";
  //        err_msg += err.what();
  //      }
  //      ++idx;
  //    }
  //  } else {
  //    err_msg += "::Receivers : not found\n";
  //  }

  QFileInfo fileInfo(dir, _path);
  if (fileInfo.exists()) {
    _uuid = fileInfo.baseName();

    PointReader reader(fileInfo);
    while (reader.hasNext()) {
      _points.push_back(reader.next());
    }

    if (json.contains("pointAmount")) {
      int jPointNum = json["pointAmount"].toInt();
      int realPointNum = static_cast<int>(_points.size());
      if (jPointNum != realPointNum) {
        err_msg += "::pointAmount : json-value != real-size  "
                   "(Note: real-size == " +
                   std::to_string(_points.size()) + ")\n";
      }
    } else {
      err_msg += "::pointAmount : not found  (Note: real value == " +
                 std::to_string(_points.size()) + ")\n";
    }

  } else {
    err_msg += "::data-file : doesn`t exist\n";
  }

  if (!err_msg.empty()) {
    err_msg += "\n";
    throw std::runtime_error(err_msg);
  }
}

SeismWell::SeismWell(const SeismWell &other)
    : _uuid(other._uuid), _path(other._path), _name(other._name),
      _points(other._points)
//    ,_receivers(other._receivers)
{

  //  for (auto &receiver : other._receivers) {
  //    _receivers.push_back(std::make_shared<SeismReceiver>(*receiver));
  //    //    _receivers.push_back(receiver);
  //  }
}

// void SeismWell::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismWell::getUuid() const { return _uuid; }

void SeismWell::setName(const QString &name) { _name = name; }

const QString &SeismWell::getName() const { return _name; }

int SeismWell::getPointsAmount() const {
  return static_cast<int>(_points.size());
}

void SeismWell::addPoint(Point point) { _points.push_back(point); }

const Point &SeismWell::getPoint(int idx) const {
  assert(0 <= idx && idx < getPointsAmount());

  return _points[static_cast<unsigned>(idx)];
}

const std::vector<Point> &SeismWell::getPoints() const { return _points; }

// void SeismWell::addReceiver(const std::shared_ptr<SeismReceiver> &receiver) {
//  //  _isSaved = false;
//  //  auto uuid = generateUuid();
//  //  receiver->setUuid(uuid);
//  _receivers.push_back(receiver);

//  //  emit addedReceiver(_receivers.back());
//}

// bool SeismWell::removeReceiver(const QUuid &uuid) {
//  for (auto itr = _receivers.begin(); itr != _receivers.end(); ++itr) {
//    if (uuid == (*itr)->getUuid()) {
//      _receivers.erase(itr);
//      //      emit removedReceiver(uuid);
//      return true;
//    }
//  }
//  return false;
//}

// int SeismWell::getReceiversAmount() const {
//  return static_cast<int>(_receivers.size());
//}

// const std::unique_ptr<SeismReceiver> &
// SeismWell::getReceiver(const QUuid &uuid) const {
//  for (auto &receiver : _receivers) {
//    if (uuid == receiver->getUuid()) {
//      return receiver;
//    }
//  }

//  // NOTE: it`s OK?
//  assert(false);
//  return *(_receivers.begin());
//}

// const std::vector<SeismReceiver const *> SeismWell::getReceivers() const {
//  //    return _receivers;
//  std::vector<SeismReceiver const *> vec;
//  for (auto &receiver : _receivers) {
//    vec.push_back(receiver.get());
//  }
//  return vec;
//}

// const std::vector<SeismReceiver *> SeismWell::getReceivers() {
//  std::vector<SeismReceiver *> vec;
//  for (auto &receiver : _receivers) {
//    vec.push_back(receiver.get());
//  }
//  return vec;
//}

// void SeismWell::removeAllReceivers() { _receivers.clear(); }

// void SeismWell::addReceiver(std::unique_ptr<SeismReceiver> receiver) {
//  auto uuid = receiver->getUuid();
//  _receivers_map[uuid] = std::move(receiver);
//  //  _receivers.push_back(std::move(receiver));
//}

// bool SeismWell::removeReceiver(const QUuid &uuid) {
//  if (_receivers_map.erase(uuid)) {
//    return true;
//  }
//  return false;
//  //  for (auto itr = _receivers.begin(); itr != _receivers.end(); ++itr) {
//  //    if (uuid == (*itr)->getUuid()) {
//  //      _receivers.erase(itr);
//  //      return true;
//  //    }
//  //  }
//  //  return false;
//}

// int SeismWell::getReceiversAmount() const {
//  return static_cast<int>(_receivers_map.size());
//  //  return static_cast<int>(_receivers.size());
//}

// const std::unique_ptr<SeismReceiver> &
// SeismWell::getReceiver(const QUuid &uuid) const {
//  return _receivers_map.at(uuid);
//}

// const std::map<QUuid, std::unique_ptr<SeismReceiver>> &
// SeismWell::getReceivers() const {
//  return _receivers_map;
//}

QJsonObject &SeismWell::writeToJson(QJsonObject &json, const QDir &dir) {
  if (_path.isEmpty()) {
    _path = _default_path;
    _path += _uuid.toString();
    _path += ".bin";
  }

  json["uuid"] = _uuid.toString();
  json["name"] = _name;
  json["path"] = _path;
  json["pointAmount"] = getPointsAmount();

  PointWriter writer(QFileInfo(dir, _path));
  for (auto point : _points) {
    writer.writePoint(point);
  }

  //  QJsonArray receiversArray;
  //  QJsonObject receiverObj;
  //  for (auto &receiver : _receivers) {
  //    receiversArray.append(receiver->writeToJson(receiverObj));
  //  }
  //  json["Receivers"] = receiversArray;

  return json;
}

} // namespace Data
