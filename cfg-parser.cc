#include "git-compat-util.hpp"
#include "config.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/foreach.hpp>
#include <fstream>


struct value_pair_t { std::string name, value; };
BOOST_FUSION_ADAPT_STRUCT(value_pair_t, (std::string, name)(std::string, value));
struct section_header_t { std::string sec, sub; };
BOOST_FUSION_ADAPT_STRUCT(section_header_t, (std::string, sec)(std::string, sub));
struct section_t { section_header_t head; std::vector<value_pair_t> values; };
BOOST_FUSION_ADAPT_STRUCT(section_t, (section_header_t, head)(std::vector<value_pair_t>, values));

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

namespace mgit {
	using std::vector;

	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;

	template <typename Iter>
	struct cfg_parser: qi::grammar<Iter, std::vector<section_t>(), ascii::space_type> {
		cfg_parser(): cfg_parser::base_type(all) {
			using qi::lit;
			using qi::lexeme;
			using qi::char_;
			using qi::eps;
			using ascii::alnum;
			using namespace qi::labels;

#define quot lit('"') // what's lit? type? object? can't find where this is defined.
			eol %= lexeme[+char_("\r\n")];
			normal_str %= lexeme[+(alnum | char_("-_/") | char_("0x80-0xFD"))];
			quot_str %= quot >> lexeme[+(alnum | char_("-_/\\+*") | char_("0x80-0xFD"))] >> quot;
			value_set %= normal_str >> '=' >> ( normal_str | quot_str );
			sec_header %= '[' >> normal_str >> ((quot >> normal_str >> quot) | eps) >> ']';
			sec %= sec_header >> +value_set;
			all %= +sec;
#undef quot
		}
		qi::rule<Iter, std::string(), ascii::space_type> eol;
		qi::rule<Iter, std::string(), ascii::space_type> normal_str;
		qi::rule<Iter, std::string(), ascii::space_type> quot_str;
		qi::rule<Iter, value_pair_t(), ascii::space_type> value_set;
		qi::rule<Iter, section_header_t(), ascii::space_type> sec_header;
		qi::rule<Iter, section_t(), ascii::space_type> sec;
		qi::rule<Iter, vector<section_t>(), ascii::space_type> all;
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


	git_config_value convert_cfg(const std::string &name, const std::string &value) {
		extern const convert_table_type convert_table;
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

	git_config_map read_config_file(const ustring &cfgfile) {
		typedef std::vector<char> buf_type;
		typedef buf_type::iterator iter_type;
		buf_type buf;
		{
			std::ifstream ifs(cfgfile.a_str().c_str());
			std::copy(std::istream_iterator<char>(ifs), std::istream_iterator<char>(), std::back_inserter(buf));
		}
		std::vector<section_t> sections;
		std::string s;
		iter_type first = buf.begin();
		mgit::cfg_parser<iter_type> parser;
		boost::spirit::qi::phrase_parse(first, buf.end(), parser, boost::spirit::ascii::space, sections);
		mgit::git_config_map cfgmap;
		mgit::build_config_map(cfgmap, sections);
		return cfgmap;
	}
}
