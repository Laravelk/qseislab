#include "model.h"

#include "data/seismchannelreceiver.h"
#include "data/seismproject.h"
#include "data/seismreceiver.h"
#include "data/seismwell.h"

#include <QFile>
#include <QTextStream>

typedef Data::SeismWell SeismWell;
typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismChannelReceiver SeismChannelReceiver;

namespace ReceiverOperation {
const unsigned Model::columnNum = 24;

Model::Model(QObject *parent) : QObject(parent) {}

std::map<QUuid, std::shared_ptr<SeismWell>> Model::getSeismReceiversFrom(
    const QString &path,
    const std::map<QUuid, std::shared_ptr<SeismWell>> &wells_map) {

  std::map<QUuid, std::shared_ptr<SeismWell>> new_well_map;
  for (auto &pair : wells_map) {
    new_well_map[pair.first] = std::make_shared<SeismWell>(*(pair.second));
  }

  try {
    csv::Parser file = csv::Parser(path.toStdString());
    checkHeaderCorrect(file);

    for (unsigned row = 0; row < file.rowCount(); ++row) {
      QString wellName =
          QString::fromStdString(file[row][21]); // Owner_Array-field
      const std::shared_ptr<SeismWell> &well =
          findWellOfName(wellName, new_well_map);

      int receiverNum = std::stoi(file[row][0]); // Instrument_Number-field
      bool isNewReceiver = true;
      const auto &receiver = getReceiver(receiverNum, well, &isNewReceiver);
      if (isNewReceiver) {
        settingRecieverInfo(receiver, file[row]);
      } else {
        checkCorrect(receiver, file[row]);
      }
      if (receiverExistInWell(receiver, well)) {
        const auto &existReceiver = getExistReceiverFromWellOfWellNum(
            well, receiver->getWellReceiverNum());
        checkCorrect(receiver, existReceiver);
      }

      auto newChannel = std::make_shared<SeismChannelReceiver>();
      settingChannelInfo(newChannel, file[row]);

      checkCorrect(newChannel, receiver);
      receiver->addChannel(std::move(newChannel));
    }
  } catch (const std::runtime_error &err) {
    new_well_map.clear();
    emit notify(err.what());
  }

  return new_well_map;
}

void Model::checkHeaderCorrect(const csv::Parser &parser) {
  if (Model::columnNum > parser.columnCount()) {
    throw std::runtime_error("number of columns not " +
                             std::to_string(Model::columnNum));
  }

  std::string err_msg;
  unsigned idx = 0;
  if ("Instrument_Number" != parser.getHeaderElement(idx)) {
    err_msg +=
        "header (idx: " + std::to_string(idx) + ") != Instrument_Number\n";
  }
  ++idx;

  if ("Instrument_Label" != parser.getHeaderElement(idx)) {
    err_msg +=
        "header (idx: " + std::to_string(idx) + ") != Instrument_Label\n";
  }
  ++idx;

  if ("Channel_Number" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Channel_Number\n";
  }
  ++idx;

  if ("Channel_Label" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Channel_Label\n";
  }
  ++idx;

  if ("North" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != North\n";
  }
  ++idx;

  if ("East" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != East\n";
  }
  ++idx;

  if ("Down" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Down\n";
  }
  ++idx;

  if ("On" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != On\n";
  }
  ++idx;

  if ("Type" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Type\n";
  }
  ++idx;

  if ("Axis_Number" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Axis_Number\n";
  }
  ++idx;

  if ("Gain" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Gain\n";
  }
  ++idx;

  if ("Sensitivity" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Sensitivity\n";
  }
  ++idx;

  if ("Vmax" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Vmax\n";
  }
  ++idx;

  if ("LowFreq" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != LowFreq\n";
  }
  ++idx;

  if ("HighFreq" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != HighFreq\n";
  }
  ++idx;

  if ("Orientation_N" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Orientation_N\n";
  }
  ++idx;

  if ("Orientation_E" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Orientation_E\n";
  }
  ++idx;

  if ("Orientation_D" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Orientation_D\n";
  }
  ++idx;

  if ("Motion" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Motion\n";
  }
  ++idx;

  if ("P_Station_Correction" != parser.getHeaderElement(idx)) {
    err_msg +=
        "header (idx: " + std::to_string(idx) + ") != P_Station_Correction\n";
  }
  ++idx;

  if ("S_Station_Correction" != parser.getHeaderElement(idx)) {
    err_msg +=
        "header (idx: " + std::to_string(idx) + ") != S_Station_Correction\n";
  }
  ++idx;

  if ("Owner_Array" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) + ") != Owner_Array\n";
  }
  ++idx;

  if ("Array_Instrument_Number" != parser.getHeaderElement(idx)) {
    err_msg += "header (idx: " + std::to_string(idx) +
               ") != Array_Instrument_Number\n";
  }
  ++idx;

  // TODO: uncomment!!!
  // NOTE: why on MacOS doesn`t work?
  //  if ("Array_Channel_Number" != parser.getHeaderElement(idx)) {
  //    err_msg +=
  //        "header (idx: " + std::to_string(idx) + ") !=
  //        Array_Channel_Number\n";
  //  }
  ++idx;

  if (!err_msg.empty()) {
    throw std::runtime_error(err_msg);
  }
}

void Model::settingRecieverInfo(const std::shared_ptr<SeismReceiver> &receiver,
                                const csv::Row &csvRow) {
  receiver->setReceiverNum(std::stoi(csvRow[0])); // Instrument_Number-field
  receiver->setName(
      QString::fromStdString(csvRow[1])); // Instrument_Label-field
  receiver->setLocation({
      std::stof(csvRow[4]),
      std::stof(csvRow[5]),
      std::stof(csvRow[6]),
  });                                    // North-field, East-field, Down-field
  receiver->setOn(std::stoi(csvRow[7])); // On-field
  receiver->setType(std::stoi(csvRow[8]));         // Type-field
  receiver->setGain(std::stoi(csvRow[10]));        // Gain-field
  receiver->setSensitivity(std::stoi(csvRow[11])); // Sensitivity-field
  receiver->setVMax(std::stoi(csvRow[12]));        // Vmax-field
  receiver->setLowFreq(std::stoi(csvRow[13]));     // LowFreq-field
  receiver->setHighFreq(std::stoi(csvRow[14]));    // HighFreq-field
  receiver->setWellReceiverNum(
      std::stoi(csvRow[22])); // Array_Instrument_Number-field
}

void Model::settingChannelInfo(
    const std::shared_ptr<SeismChannelReceiver> &channel,
    const csv::Row &csvRow) {
  channel->setName(QString::fromStdString(csvRow[3])); // Channel_Label-field
  channel->setChannelNum(std::stoi(csvRow[2]));        // Channel_Number-field
  channel->setAxisNum(std::stoi(csvRow[9]));           // Axis_Number-field
  channel->setOrientation(
      {std::stof(csvRow[15]), std::stof(csvRow[16]), std::stof(csvRow[17])});
  // Orientation_N-field, Orientation_E-field,
  // Orientation_D-field
  channel->setMotion(std::stoi(csvRow[18])); // Motion-field
  channel->setPStationCorrection(
      std::stoi(csvRow[19])); // P_Station_Correction-field
  channel->setSStationCorrection(
      std::stoi(csvRow[20])); // S_Station_Correction-field
  channel->setWellChannelNum(
      std::stoi(csvRow[23])); // Array_Channel_Number-field
}

const std::shared_ptr<SeismReceiver> &
Model::getReceiver(const int receiverNum,
                   const std::shared_ptr<SeismWell> &well, bool *isNew) {

  for (auto &receiver : well->getReceivers()) {
    if (receiverNum == receiver->getReceiverNum()) {
      *isNew = false;
      return receiver;
    }
  }

  *isNew = true;
  auto new_receiver = std::make_shared<SeismReceiver>();
  well->addReceiver(std::move(new_receiver));
  return well->getReceivers().back();
}

const std::shared_ptr<Data::SeismWell> &Model::findWellOfName(
    const QString &wellName,
    std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map) {
  for (auto &uuid_well : wells_map) {
    if (wellName == (uuid_well.second)->getName()) {
      return uuid_well.second;
    }
  }

  throw std::runtime_error("well with name \"" + wellName.toStdString() +
                           "\" not found");
}

bool Model::receiverExistInWell(
    const std::shared_ptr<Data::SeismReceiver> &receiver,
    const std::shared_ptr<Data::SeismWell> &well) {
  for (auto &existReceiver : well->getReceivers()) {
    if (existReceiver->getWellReceiverNum() == receiver->getWellReceiverNum()) {
      return true;
    }
  }

  return false;
}

const std::shared_ptr<Data::SeismReceiver> &
Model::getExistReceiverFromWellOfWellNum(
    const std::shared_ptr<Data::SeismWell> &well, const int wellReceiverNum) {
  for (auto &receiver : well->getReceivers()) {
    if (wellReceiverNum == receiver->getWellReceiverNum()) {
      return receiver;
    }
  }

  throw std::runtime_error("Receiver with " + std::to_string(wellReceiverNum) +
                           " wellReceiverNum not found in well (" +
                           well->getName().toStdString() + ")");
}

void Model::checkCorrect(const std::shared_ptr<Data::SeismReceiver> &receiver,
                         const csv::Row &csvRow) {
  std::string err_msg;
  if (receiver->getReceiverNum() != std::stoi(csvRow[0])) {
    err_msg += "!=receiverNum\n";
  } // Instrument_Number-field
  if (receiver->getName() != QString::fromStdString(csvRow[1])) {
    err_msg += "!=name\n";
  } // Instrument_Label-field
  if (receiver->getLocation() != Data::Point{std::stof(csvRow[4]),
                                             std::stof(csvRow[5]),
                                             std::stof(csvRow[6])}) {
    err_msg += "!=location\n";
  }
  if (receiver->getOn() != std::stoi(csvRow[7])) {
    err_msg += "!=on\n";
  } // On-field
  if (receiver->getType() != std::stoi(csvRow[8])) {
    err_msg += "!=type\n";
  } // Type-field
  if (receiver->getGain() != std::stoi(csvRow[10])) {
    err_msg += "!=gain\n";
  } // Gain-field
  if (receiver->getSensitivity() != std::stoi(csvRow[11])) {
    err_msg += "!=Sensitivity\n";
  } // Sensitivity-field
  if (receiver->getVMax() != std::stoi(csvRow[12])) {
    err_msg += "!=vmax\n";
  } // Vmax-field
  if (receiver->getLowFreq() != std::stoi(csvRow[13])) {
    err_msg += "!=lowFreq\n";
  } // LowFreq-field
  if (receiver->getHighFreq() != std::stoi(csvRow[14])) {
    err_msg += "!=highFreq\n";
  } // HighFreq-field
  if (receiver->getWellReceiverNum() != std::stoi(csvRow[22])) {
    err_msg += "!=wellReceiverNum\n";
  } // Array_Instrument_Number-field

  if (!err_msg.empty()) {
    throw std::runtime_error("from checkCorrect(receiver, csvRow):\n" +
                             err_msg);
  }
}

void Model::checkCorrect(
    const std::shared_ptr<Data::SeismReceiver> &newReceiever,
    const std::shared_ptr<Data::SeismReceiver> &existReceiver) {

  std::string err_msg;

  if (newReceiever->getReceiverNum() != existReceiver->getReceiverNum()) {
    err_msg += "!=receiverNum\n";
  }

  if (newReceiever->getName() != existReceiver->getName()) {
    err_msg += "!=name\n";
  }

  // TODO: check: location, on, type, gain, sensitivity, vmax, lowfreq,
  // highfreq

  if (!err_msg.empty()) {
    throw std::runtime_error("from checkCorrect(newRec, existRec):\n" +
                             err_msg);
  }
}

void Model::checkCorrect(
    const std::shared_ptr<Data::SeismChannelReceiver> &channel,
    const std::shared_ptr<Data::SeismReceiver> &receiver) {

  std::string err_msg;

  // TODO: realize

  if (!err_msg.empty()) {
    throw std::runtime_error("from checkCorrect(channel, receiver):\n" +
                             err_msg);
  }
}

} // namespace ReceiverOperation
