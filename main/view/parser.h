#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <exception>
#include <fstream>
#include <string>

#include <QDateTime>
#include <QString>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace TableFilterParsing {

struct op_or {};
struct op_and {};
struct op_not {};

struct op_more {};
struct op_less {};
struct op_eq_more {};
struct op_eq_less {};

using var = std::string;

template <typename tag, typename param_t> struct binop;
template <typename tag, typename param_t> struct unop;
template <typename tag, typename param_t> struct param_unop;

template <typename param_t>
using expr =
    boost::variant<var, boost::recursive_wrapper<unop<op_not, param_t>>,
                   boost::recursive_wrapper<binop<op_and, param_t>>,
                   boost::recursive_wrapper<binop<op_or, param_t>>,
                   param_unop<op_more, param_t>, param_unop<op_less, param_t>,
                   param_unop<op_eq_more, param_t>,
                   param_unop<op_eq_less, param_t>>;

template <typename tag, typename param_t> struct param_unop {
  explicit param_unop(const param_t &p) : param1(p) {}
  param_t param1;
};

template <typename tag, typename param_t> struct binop {
  explicit binop(const expr<param_t> &l, const expr<param_t> &r)
      : oper1(l), oper2(r) {}
  expr<param_t> oper1, oper2;
};

template <typename tag, typename param_t> struct unop {
  explicit unop(const expr<param_t> &o) : oper1(o) {}
  expr<param_t> oper1;
};

template <typename param_t> struct Evaluator : boost::static_visitor<bool> {
public:
  Evaluator(const param_t &p) : param(p) {}

  static bool evaluate(const expr<param_t> &e, const param_t &p) {
    return boost::apply_visitor(Evaluator(p), e);
  }

  bool operator()(const var &v) const {
    if (v == "T" || v == "t" || v == "true" || v == "True") {
      return true;
    } else if (v == "F" || v == "f" || v == "false" || v == "False") {
      return false;
    }
    throw std::exception();
  }

  bool operator()(const binop<op_and, param_t> &b) const {
    return recurse(b.oper1) && recurse(b.oper2);
  }
  bool operator()(const binop<op_or, param_t> &b) const {
    return recurse(b.oper1) || recurse(b.oper2);
  }
  bool operator()(const unop<op_not, param_t> &u) const {
    return !recurse(u.oper1);
  }
  bool operator()(const param_unop<op_more, param_t> &pu) const {
    return this->param > pu.param1;
  }
  bool operator()(const param_unop<op_less, param_t> &pu) const {
    return this->param < pu.param1;
  }
  bool operator()(const param_unop<op_eq_more, param_t> &pu) const {
    return this->param >= pu.param1;
  }
  bool operator()(const param_unop<op_eq_less, param_t> &pu) const {
    return this->param <= pu.param1;
  }

private:
  param_t param;

  template <typename T> bool recurse(T const &v) const {
    return boost::apply_visitor(*this, v);
  }
};

template <typename param_t, typename It, typename Skipper = qi::space_type>
struct Parser : qi::grammar<It, expr<param_t>(), Skipper> {
public:
  Parser() : Parser::base_type(expr_) {
    using namespace qi;

    expr_ = or_.alias();

    or_ = (and_ >> '|' >>
           or_)[_val = phx::construct<binop<op_or, param_t>>(_1, _2)] |
          and_[_val = _1];
    and_ = (not_ >> '&' >>
            and_)[_val = phx::construct<binop<op_and, param_t>>(_1, _2)] |
           not_[_val = _1];
    not_ = ('!' > simple)[_val = phx::construct<unop<op_not, param_t>>(_1)] |
           simple[_val = _1];

    simple = (('(' > expr_ > ')') | eq_more_);
    eq_more_ =
        ((">=" >
          param_)[_val = phx::construct<param_unop<op_eq_more, param_t>>(_1)] |
         more_[_val = _1]);

    more_ = (('>' >
              param_)[_val = phx::construct<param_unop<op_more, param_t>>(_1)] |
             eq_less_[_val = _1]);
    eq_less_ =
        (("<=" >
          param_)[_val = phx::construct<param_unop<op_eq_less, param_t>>(_1)] |
         less_[_val = _1]);

    less_ = (('<' >
              param_)[_val = phx::construct<param_unop<op_less, param_t>>(_1)] |
             var_[_val = _1]);

    var_ = qi::lexeme[+(alpha | digit)];

    if constexpr (std::is_same_v<QString, param_t>) {
      param_ = (as_string[+(alpha | digit)])[_val = phx::bind(
                                                 &QString::fromStdString, _1)];
    } else if constexpr (std::is_same_v<QDate, param_t>) {
      param_ = (as_string[digit >> digit >> ascii::char_('.') >> digit >>
                          digit >> ascii::char_('.') >> digit >> digit])
          [_val = phx::bind(
               [](const auto &str) -> QDate {
                 return QDate::fromString(QString::fromStdString(str),
                                          "dd.MM.yy");
               },
               _1)];
    } else if constexpr (std::is_same_v<QTime, param_t>) {
      param_ =
          (as_string[digit >> digit >> ascii::char_(':') >> digit >> digit])
              [_val = phx::bind(
                   [](auto str) -> QTime {
                     return QTime::fromString(QString::fromStdString(str),
                                              "hh:mm");
                   },
                   _1)];
    } else if constexpr (std::is_same_v<float, param_t>) {
      param_ = qi::float_;
    } else if constexpr (std::is_same_v<int, param_t>) {
      param_ = qi::int_;
    } else if constexpr (std::is_same_v<std::string, param_t>) {
      param_ = as_string[+alnum];
    } else {
      assert(false);
    }

    BOOST_SPIRIT_DEBUG_NODE(expr_);
    BOOST_SPIRIT_DEBUG_NODE(or_);
    BOOST_SPIRIT_DEBUG_NODE(and_);
    BOOST_SPIRIT_DEBUG_NODE(not_);
    BOOST_SPIRIT_DEBUG_NODE(simple);
    BOOST_SPIRIT_DEBUG_NODE(var_);

    BOOST_SPIRIT_DEBUG_NODE(more_);
    BOOST_SPIRIT_DEBUG_NODE(less_);
    BOOST_SPIRIT_DEBUG_NODE(eq_more_);
    BOOST_SPIRIT_DEBUG_NODE(eq_less_);
    BOOST_SPIRIT_DEBUG_NODE(param_);
  }

  static bool evaluate(const expr<param_t> &e, const param_t &p) {
    return boost::apply_visitor(Evaluator<param_t>(p), e);
  }

private:
  qi::rule<It, var(), Skipper> var_;
  qi::rule<It, expr<param_t>(), Skipper> not_, and_, or_, simple, expr_;

  qi::rule<It, expr<param_t>(), Skipper> more_, less_, eq_more_, eq_less_;
  qi::rule<It, param_t(), Skipper> param_;
};

} // namespace TableFilterParsing
