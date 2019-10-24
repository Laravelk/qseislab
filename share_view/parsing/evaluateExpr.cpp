#include "evaluateExpr.h"

#include "parser.h"

#include <iostream>

namespace TableFilterParsing {

template bool parseAndEvaluateExpr<int>(const std::string &,
                                        const std::string &);
template bool parseAndEvaluateExpr<float>(const std::string &,
                                          const std::string &);
template bool parseAndEvaluateExpr<std::string>(const std::string &,
                                                const std::string &);
template bool parseAndEvaluateExpr<QDate>(const std::string &,
                                          const std::string &);
template bool parseAndEvaluateExpr<QTime>(const std::string &,
                                          const std::string &);
template bool parseAndEvaluateExpr<QString>(const std::string &,
                                            const std::string &);

template <typename param_t> param_t castToParam(const std::string &value) {
  if constexpr (std::is_same_v<QString, param_t>) {
    return QString::fromStdString(value);
  } else if constexpr (std::is_same_v<std::string, param_t>) {
    return value;
  } else if constexpr (std::is_same_v<QDate, param_t>) {
    return QDate::fromString(QString::fromStdString(value), "dd.MM.yy");
  } else if constexpr (std::is_same_v<QTime, param_t>) {
    return QTime::fromString(QString::fromStdString(value), "hh::mm");
  } else if constexpr (std::is_same_v<int, param_t>) {
    return std::stoi(value);
  } else if constexpr (std::is_same_v<float, param_t>) {
    return std::stof(value);
  } else {
    assert(false);
  }
}

template <typename param_t>
bool parseAndEvaluateExpr(const std::string &expr,
                          const std::string &std_param) {
  typedef std::string::const_iterator It;
  It phrase_begin(expr.begin());
  It phrase_end(expr.end());

  TableFilterParsing::Parser<param_t, It> parser;
  TableFilterParsing::expr<param_t> filter;

  bool ok = true;
  try {
    ok = qi::phrase_parse(phrase_begin, phrase_end, parser, qi::space, filter);

  } catch (...) {
    std::cerr << "catch exc from phrase_parse" << std::endl;
    return false;
  }

  std::cerr << "ok == " << ok << std::endl;

  try {
    param_t param = castToParam<param_t>(std_param);
    return TableFilterParsing::evaluate(filter, param);
  } catch (...) {
    return false;
  }
}

} // namespace TableFilterParsing
