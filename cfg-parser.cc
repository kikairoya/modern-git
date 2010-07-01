#include "git-compat-util.hpp"
#include "config.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/foreach.hpp>
#include <fstream>


struct value_pair_t { std::string name, value; };
BOOST_FUSION_ADAPT_STRUCT(value_pair_t, (std::string, name)(std::string, value));
struct section_header_t { std::string sec, sub; };
BOOST_FUSION_ADAPT_STRUCT(section_header_t, (std::string, sec)(std::string, sub));
struct section_t { section_header_t head; std::vector<value_pair_t> values; };
BOOST_FUSION_ADAPT_STRUCT(section_t, (section_header_t, head)(std::vector<value_pair_t>, values));

namespace {
	std::ostream &operator <<(std::ostream &os, const value_pair_t &val) {
		return os << val.name << '=' << val.value;
	}
	std::ostream &operator <<(std::ostream &os, const std::vector<value_pair_t> &values) {
		BOOST_FOREACH (const value_pair_t &v, values) { os << v << '\n'; }
		return os;
	}
	std::ostream &operator <<(std::ostream &os, const section_header_t &head) {
		os << head.sec;
		if (!head.sub.empty()) os << ' ' << '"' << head.sub << '"';
		return os;
	}
	std::ostream &operator <<(std::ostream &os, const section_t &sec) {
		return os << '[' << sec.head << ']' << '\n' << sec.values << '\n';
	}
	std::ostream &operator <<(std::ostream &os, const std::vector<section_t> &vsec) {
		BOOST_FOREACH (const section_t &v, vsec) { os << v << '\n'; }
		return os;
	}
}

namespace mgit {
	using std::vector;

	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;

	template <typename Iter>
	struct skip_parser: qi::grammar<Iter> {
		skip_parser(): skip_parser::base_type(all) {
			using qi::eol;
			using qi::lexeme;
			using qi::lit;
			using ascii::char_;
			using ascii::space;

			comment = lexeme[char_(";#") >> *(char_-'\n'-'\r') >> eol];
			all = space | comment;
		}
		qi::rule<Iter> comment;
		qi::rule<Iter> all;
	};

	template <typename Iter>
	struct blank_parser: qi::grammar<Iter> {
		blank_parser(): blank_parser::base_type(all) {
			using ascii::blank;

			all = blank;
		}
		qi::rule<Iter> all;
	};

	template <typename Iter>
	struct cfg_parser: qi::grammar<Iter, std::vector<section_t>(), skip_parser<Iter> > {
		cfg_parser(): cfg_parser::base_type(all) {
			using qi::lit;
			using qi::lexeme;
			using qi::eol;
			using qi::no_skip;
			using qi::on_error;
			using qi::fail;
			using ascii::alnum;
			using ascii::char_;
			using phoenix::val;
			using phoenix::construct;
			using namespace qi::labels;

			spc = char_(" \t");
			normal_chr %= alnum | char_("-_/") | char_("0x80-0xFD");
			normal_str %= lexeme[+normal_chr];
			value_str %= lexeme[+(normal_chr | lit('.'))];
			quot_str %= lit('"') >> lexeme[*(normal_chr | char_("\\+* \t."))] > lit('"');
			value_set %= normal_str >> '=' >> ( value_str | quot_str );
			sec_header %= '[' >> normal_str >> no_skip[('.' >> normal_str) | -(+spc >> quot_str)] > ']';
			sec %= sec_header >> *value_set;
			all %= *sec;

			spc.name("spaces");
			normal_chr.name("normal_chr");
			normal_str.name("normal_str");
			quot_str.name("quot_str");
			value_set.name("value_set");
			sec_header.name("sec_header");
			sec.name("sec");
			all.name("all");
			on_error<fail> (
				all,
				std::cout
				<< val("Error! Expecting ")
				<< _4
				<< val(" here: \"")
				<< construct<std::string>(_3, _2)
				<< val("\"")
				<< std::endl
				);
		}
		qi::rule<Iter> spc;
		qi::rule<Iter, char()> normal_chr;
		qi::rule<Iter, std::string()> normal_str;
		qi::rule<Iter, std::string()> value_str;
		qi::rule<Iter, std::string()> quot_str;
		qi::rule<Iter, value_pair_t(), blank_parser<Iter> > value_set;
		qi::rule<Iter, section_header_t(), blank_parser<Iter> > sec_header;
		qi::rule<Iter, section_t(), skip_parser<Iter> > sec;
		qi::rule<Iter, vector<section_t>(), skip_parser<Iter> > all;
	};

	std::ostream &operator <<(std::ostream &os, const git_config_map::value_type &v) {
		os << v.first.section << '.';
		if (!v.first.subsection.empty()) os << v.first.subsection << '.';
		return os << v.first.name << '=' << v.second;
	}
	std::ostream &operator <<(std::ostream &os, const git_config_map &v) {
		BOOST_FOREACH (git_config_map::value_type vv, v) { os << vv << '\n'; }
		return os;
	}

	extern const convert_table_type convert_table;
	namespace {
		git_config_value convert_cfg(const std::string &name, const std::string &value) {
			convert_table_type::const_iterator p = convert_table.find(name);
			return p==convert_table.end() ? value : p->second(value);
		}
		void build_config_map(git_config_map &cfg, const std::vector<section_t> &sects) {
			BOOST_FOREACH (const section_t &s, sects) {
				BOOST_FOREACH (const value_pair_t &v, s.values) {
					const std::string n = s.head.sec + (s.head.sub.empty() ? "." : ".*.") + v.name;
					const git_config_name h(s.head.sec, s.head.sub, v.name);
					cfg.insert(std::make_pair(h, convert_cfg(n, v.value)));
				}
			}
		}
	}

	git_config_map read_config_file(const ustring &cfgfile) {
		typedef std::vector<char> buf_type;
		typedef buf_type::iterator iter_type;
		buf_type buf;
		{
			std::ifstream ifs(cfgfile.a_str().c_str());
			ifs.unsetf(std::ios_base::skipws);
			std::copy(std::istream_iterator<char>(ifs), std::istream_iterator<char>(), std::back_inserter(buf));
		}
		std::vector<section_t> sections;
		std::string s;
		iter_type first = buf.begin();
		mgit::cfg_parser<iter_type> parser;
		mgit::skip_parser<iter_type> skipper;
		boost::spirit::qi::phrase_parse(first, buf.end(), parser, skipper, sections);
		mgit::git_config_map cfgmap;
		mgit::build_config_map(cfgmap, sections);
		return cfgmap;
	}
}
