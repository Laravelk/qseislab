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

template <typename param_t, typename It, typename Skipper = qi::space_type>
struct Parser : qi::grammar<It, expr(), Skipper> {
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
      param_ =
          (as_string[+(alpha | digit)]) [_val = phx::bind(&QString::fromStdString, _1)];
    } else if constexpr (std::is_same_v<QDate, param_t>) {
      param_ = qi::lexeme[digit >> digit >> lit('.') >> digit >> digit >>
                          lit('.') >> digit >> digit];

    } else if constexpr (std::is_same_v<QTime, param_t>) {
      param_ = qi::lexeme[digit >> digit >> lit(':') >> digit >> digit];
    } else if constexpr (std::is_same_v<float, param_t>) {
      param_ = qi::float_;
    } else if constexpr (std::is_same_v<int, param_t>) {
      param_ = qi::int_;
    } else if constexpr (std::is_same_v<std::string, param_t>) {
      param_ = qi::lexeme[+(alpha | digit)];
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

private:
  qi::rule<It, var(), Skipper> var_;
  qi::rule<It, expr(), Skipper> not_, and_, or_, simple, expr_;

  qi::rule<It, expr(), Skipper> more_, less_;
  qi::rule<It, param_t(), Skipper> param_;
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

} // namespace TableFilterParsing

// int main(int argc, char const *argv[]) {
//	if (2 > argc) {
//        std::cerr << "Usage: " << argv[0] << " <test_data>" << std::endl;
//        return 1;
//    }

//    std::ifstream file(argv[1]);
//    if(!file) {
//        std::cerr << argv[1] << " not opened" << std::endl;
//        return 2;
//    }

//    std::vector<std::string> inputs;
//    while(!file.eof()) {
//        std::string str;
//        getline(file, str);
//        inputs.push_back(str);
//    }
//    file.close();

//    for (int i = 0; i < inputs.size(); ++i) {
//        typedef std::string::const_iterator It;
//        It phrase_begin(inputs[i].begin());
//        It phrase_end(inputs[i].end());
//        TableFilterParsing::Parser<int, It> parser;

//        try {
//            TableFilterParsing::expr result;
//            bool ok = qi::phrase_parse(phrase_begin, phrase_end, parser >
//            ';',qi::space,result);

//            if (!ok){
//                std::cerr << "invalid input\n";
//            }
//            else {
//                std::cout << "evaluated:\t" <<
//                TableFilterParsing::Evaluator<int>::evaluate(result, 10) <<
//                "\n\n";
//            }
//        } catch (const qi::expectation_failure<It>& e) {
//            std::cerr << "expectation_failure at '" << std::string(e.first,
//            e.last) << "'\n\n";
//        }

//        if (phrase_end != phrase_begin) {
//            std::cerr << "unparsed: '" << std::string(phrase_begin,
//            phrase_end) << "'\n\n";
//        }
//    }
//	return 0;
//}
