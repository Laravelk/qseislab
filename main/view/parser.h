#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_wrapper.hpp>
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

typedef std::string var;

template <typename tag> struct binop;
template <typename tag> struct unop;
template <typename tag, typename param_t> struct param_unop;

typedef boost::variant<
    var, boost::recursive_wrapper<unop<op_not>>,
    boost::recursive_wrapper<binop<op_and>>,
    boost::recursive_wrapper<binop<op_or>>, param_unop<op_more, QString>,
    param_unop<op_more, QDate>, param_unop<op_more, QTime>,
    param_unop<op_more, float>, param_unop<op_more, int>,
    param_unop<op_more, std::string>, param_unop<op_less, QString>,
    param_unop<op_less, QDate>, param_unop<op_less, QTime>,
    param_unop<op_less, float>, param_unop<op_less, int>,
    param_unop<op_less, std::string>>
    expr;

template <typename tag, typename param_t> struct param_unop {
  explicit param_unop(const param_t &p) : param1(p) {}
  param_t param1;
};

template <typename tag> struct binop {
  explicit binop(const expr &l, const expr &r) : oper1(l), oper2(r) {}
  expr oper1, oper2;
};

template <typename tag> struct unop {
  explicit unop(const expr &o) : oper1(o) {}
  expr oper1;
};

template <typename param_t> struct Evaluator : boost::static_visitor<bool> {
public:
  Evaluator(const param_t &p) : param(p) {}

  static bool evaluate(const expr &e, const param_t &p) {
    return boost::apply_visitor(Evaluator(p), e);
  }

  bool operator()(const var &v) const {
    if (v == "T" || v == "t" || v == "true" || v == "True") {
      return true;
    } else if (v == "F" || v == "f" || v == "false" || v == "False") {
      return false;
    }
    return boost::lexical_cast<bool>(v);
  }

  bool operator()(const binop<op_and> &b) const {
    return recurse(b.oper1) && recurse(b.oper2);
  }
  bool operator()(const binop<op_or> &b) const {
    return recurse(b.oper1) || recurse(b.oper2);
  }
  bool operator()(const unop<op_not> &u) const { return !recurse(u.oper1); }

  bool operator()(const param_unop<op_more, QString> &pu) const {
    if constexpr (std::is_same_v<param_t, QString>) {
      return this->param > pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_more, QDate> &pu) const {
    if constexpr (std::is_same_v<param_t, QDate>) {
      return this->param > pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_more, QTime> &pu) const {
    if constexpr (std::is_same_v<param_t, QTime>) {
      return this->param > pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_more, float> &pu) const {
    if constexpr (std::is_same_v<param_t, float>) {
      return this->param > pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_more, int> &pu) const {
    if constexpr (std::is_same_v<param_t, int>) {
      return this->param > pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_more, std::string> &pu) const {
    if constexpr (std::is_same_v<param_t, std::string>) {
      return this->param > pu.param1;
    } else {
      assert(false);
    }
  }

  bool operator()(const param_unop<op_less, QString> &pu) const {
    if constexpr (std::is_same_v<param_t, QString>) {
      return this->param < pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_less, QDate> &pu) const {
    if constexpr (std::is_same_v<param_t, QDate>) {
      return this->param < pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_less, QTime> &pu) const {
    if constexpr (std::is_same_v<param_t, QTime>) {
      return this->param < pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_less, float> &pu) const {
    if constexpr (std::is_same_v<param_t, float>) {
      return this->param < pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_less, int> &pu) const {
    if constexpr (std::is_same_v<param_t, int>) {
      return this->param < pu.param1;
    } else {
      assert(false);
    }
  }
  bool operator()(const param_unop<op_less, std::string> &pu) const {
    if constexpr (std::is_same_v<param_t, std::string>) {
      return this->param < pu.param1;
    } else {
      assert(false);
    }
  }

private:
  param_t param;

  template <typename T> bool recurse(T const &v) const {
    return boost::apply_visitor(*this, v);
  }
};

template <typename It, typename Skipper = qi::space_type>
struct AbstractParser : qi::grammar<It, expr(), Skipper> {};

template <typename param_t, typename It, typename Skipper = qi::space_type>
struct Parser : AbstractParser<It, Skipper> {
public:
  Parser() : Parser::base_type(expr_) {
    using namespace qi;

    expr_ = or_.alias();

    or_ = (and_ >> '|' >> or_)[_val = phx::construct<binop<op_or>>(_1, _2)] |
          and_[_val = _1];
    and_ = (not_ >> '&' >> and_)[_val = phx::construct<binop<op_and>>(_1, _2)] |
           not_[_val = _1];
    not_ = ('!' > simple)[_val = phx::construct<unop<op_not>>(_1)] |
           simple[_val = _1];

    simple = (('(' > expr_ > ')') | more_);
    more_ = (('>' >
              param_)[_val = phx::construct<param_unop<op_more, param_t>>(_1)] |
             less_[_val = _1]);
    less_ = (('<' >
              param_)[_val = phx::construct<param_unop<op_less, param_t>>(_1)] |
             var_[_val = _1]);

    var_ = qi::lexeme[+(alpha | digit)];

    if constexpr (std::is_same_v<QString, param_t>) {
      param_ = (as_string[+(alpha | digit)])[_val = phx::bind(
                                                 &QString::fromStdString, _1)];
    } else if constexpr (std::is_same_v<QDate, param_t>) {
      //      param_ = (as_string[digit >> digit >> lit('.') >> digit >> digit
      //      >>
      //                          lit('.') >> digit >> digit])
      //          [_val = phx::bind(&QDate::fromString,
      //                            phx::bind(&QString::fromStdString, _1))];
    } else if constexpr (std::is_same_v<QTime, param_t>) {
      //      param_ = (as_string[digit >> digit >> lit(':') >> digit >> digit])
      //          [_val = phx::bind(&QTime::fromString,
      //                            phx::bind(&QString::fromStdString, _1))];
    } else if constexpr (std::is_same_v<float, param_t>) {
      param_ = qi::float_;
    } else if constexpr (std::is_same_v<int, param_t>) {
      param_ = qi::int_;
    } else if constexpr (std::is_same_v<std::string, param_t>) {
      param_ = as_string[+(alpha | digit)];
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
    BOOST_SPIRIT_DEBUG_NODE(param_);
  }

  static bool evaluate(const expr &e, const param_t &p) {
    return boost::apply_visitor(Evaluator<param_t>(p), e);
  }

private:
  qi::rule<It, var(), Skipper> var_;
  qi::rule<It, expr(), Skipper> not_, and_, or_, simple, expr_;

  qi::rule<It, expr(), Skipper> more_, less_;
  qi::rule<It, param_t(), Skipper> param_;
};

} // namespace TableFilterParsing
