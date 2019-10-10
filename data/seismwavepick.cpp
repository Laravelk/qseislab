#include "seismwavepick.h"

namespace Data {
const std::string &SeismWavePick::to_string(Type enumVal) {
  static const std::map<Type, std::string> enumStringsMap =
      _make_enum_strings_map();
  auto it = enumStringsMap.find(enumVal);
  static std::string emptyString;
  if (enumStringsMap.end() == it)
    return emptyString;
  return it->second;
}
SeismWavePick::Type SeismWavePick::from_string(const std::string &value) {
  static const std::map<std::string, Type> stringsEnumMap =
      _make_strings_enum_map();
  std::map<std::string, Type>::const_iterator it = stringsEnumMap.find(value);
  if (stringsEnumMap.end() == it) {
    return static_cast<Type>(0);
  }
  return it->second;
}

SeismWavePick::SeismWavePick() {}

SeismWavePick::SeismWavePick(Type type, int arrival)
    : _type(type), _arrival(arrival), _polarizationLeftBorder(arrival - 20000),
      _polarizationRightBorder(arrival + 20000) {}

SeismWavePick::SeismWavePick(const QJsonObject &json) {
  std::string err_msg;
  if (json.contains("type")) {
    _type = from_string(json["type"].toString().toStdString());
    if (0 == _type) {
      err_msg += "::type : unknown this type\n";
    }
  } else {
    err_msg += "::type : not found\n";
  }

  if (json.contains("arrival")) {
    _arrival = json["arrival"].toInt();
  } else {
    err_msg += "::arrival : not found\n";
  }

  if (json.contains("polarizationLeftBorder")) {
    _polarizationLeftBorder = json["polarizationLeftBorder"].toInt();
  } else {
    err_msg += "::polarizationLeftBorder : not found\n";
  }

  if (json.contains("polarizationRightBorder")) {
    _polarizationRightBorder = json["polarizationRightBorder"].toInt();
  } else {
    err_msg += "::polarizationRightBorder : not found\n";
  }

  if (!err_msg.empty()) {
    err_msg += "\n";
    throw std::runtime_error(err_msg);
  }
}

SeismWavePick::Type SeismWavePick::getType() const { return _type; }

void SeismWavePick::setType(SeismWavePick::Type type) { _type = type; }

int SeismWavePick::getArrival() const { return _arrival; }

void SeismWavePick::setArrival(int arrival) { _arrival = arrival; }

int SeismWavePick::getPolarizationLeftBorder() const {
  return _polarizationLeftBorder;
}

void SeismWavePick::setPolarizationLeftBorder(int polarizationLeftBorder) {
  _polarizationLeftBorder = polarizationLeftBorder;
}

int SeismWavePick::getPolarizationRightBorder() const {
  return _polarizationRightBorder;
}

void SeismWavePick::setPolarizationRightBorder(int polarizationRightBorder) {
  _polarizationRightBorder = polarizationRightBorder;
}

QJsonObject &SeismWavePick::writeToJson(QJsonObject &json) const {
  json["type"] = QString::fromStdString(to_string(_type));
  json["arrival"] = _arrival;
  json["polarizationLeftBorder"] = _polarizationLeftBorder;
  json["polarizationRightBorder"] = _polarizationRightBorder;

  return json;
}

std::map<SeismWavePick::Type, std::string>
SeismWavePick::_make_enum_strings_map() {
  std::map<Type, std::string> enumStringsMap;
  enumStringsMap.insert(std::make_pair(PWAVE, "PWAVE"));
  enumStringsMap.insert(std::make_pair(SWAVE, "SWAVE"));
  return enumStringsMap;
}

std::map<std::string, SeismWavePick::Type>
SeismWavePick::_make_strings_enum_map() {
  std::map<std::string, Type> stringsEnumMap;
  stringsEnumMap.insert(std::make_pair("PWAVE", PWAVE));
  stringsEnumMap.insert(std::make_pair("SWAVE", SWAVE));
  return stringsEnumMap;
}

} // namespace Data
