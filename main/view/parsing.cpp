#include "parsing.h"

#include "parser.h"

template bool calculate<int>(const std::string &, const std::string &);

template bool calculate<float>(const std::string &, const std::string &);

template bool calculate<std::string>(const std::string &, const std::string &);

template bool calculate<QDate>(const std::string &, const std::string &);

template bool calculate<QTime>(const std::string &, const std::string &);

template bool calculate<QString>(const std::string &, const std::string &);

template <typename param_t> param_t castToParam(const std::string &value) {
  if constexpr (std::is_same_v<QString, param_t>) {
    return QString::fromStdString(value);
  } else if constexpr (std::is_same_v<std::string, param_t>) {
    return value;
  } else if constexpr (std::is_same_v<QDate, param_t>) {
    return QDate::fromString(QString::fromStdString(value));
  } else if constexpr (std::is_same_v<QTime, param_t>) {
    return QTime::fromString(QString::fromStdString(value));
  } else if constexpr (std::is_same_v<int, param_t>) {
    return std::stoi(value);
  } else if constexpr (std::is_same_v<float, param_t>) {
    return std::stof(value);
  } else {
    assert(false);
  }
}

template <typename param_t>
bool calculate(const std::string &expr, const std::string &std_param) {
  typedef std::string::const_iterator It;
  It phrase_begin(expr.begin());
  It phrase_end(expr.end());

  TableFilterParsing::Parser<param_t, It> parser;
  TableFilterParsing::expr<param_t> filter;

  bool ok = true;
  try {
    ok = qi::phrase_parse(phrase_begin, phrase_end, parser, qi::space, filter);
  } catch (...) {
    std::cerr << "bad parse" << std::endl;
    return false;
  }
  if (0 == ok) {
    std::cout << "ok == 0" << std::endl;
    return false;
  }

  bool eval;
  try {
    param_t param = castToParam<param_t>(std_param);
    eval = TableFilterParsing::evaluate(filter, param);
  } catch (...) {
    std::cerr << "Bad eval" << std::endl;
    return false;
  }

  std::cout << "eval == " << eval << std::endl;

  return eval;
}
