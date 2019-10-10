#pragma once

#include <map>

#include <QJsonObject>

namespace Data {
class SeismWavePick {
public:
  enum Type { PWAVE = 1, SWAVE };
  static const std::string &to_string(Type);
  static Type from_string(const std::string &);

  explicit SeismWavePick();
  explicit SeismWavePick(Type, int arrival);

  explicit SeismWavePick(const QJsonObject &json) noexcept(false);

  Type getType() const;
  void setType(Type);

  int getArrival() const;
  void setArrival(int);

  int getPolarizationLeftBorder() const;
  void setPolarizationLeftBorder(int);

  int getPolarizationRightBorder() const;
  void setPolarizationRightBorder(int);

  QJsonObject &writeToJson(QJsonObject &) const noexcept(false);

private:
  Type _type;

  int _arrival;
  int _polarizationLeftBorder;
  int _polarizationRightBorder;

  static std::map<Type, std::string> _make_enum_strings_map();
  static std::map<std::string, Type> _make_strings_enum_map();
};

} // namespace Data
