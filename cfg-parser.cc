#include "git-compat-util.hpp"
#include "config.hpp"
#include <boost/foreach.hpp>
#include <fstream>

namespace mgit {
	struct MGIT_DECL git_cfgparser_exception: git_exception { };

	namespace parser {
		struct value_pair_t { std::string name, value; };
		struct section_header_t { std::string sec, sub; };
		struct section_t { section_header_t head; std::vector<value_pair_t> values; };

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

		enum class_tag {
			custom_set_tag,
			single_char_tag,
			alnum_tag,
			alpha_tag,
			space_tag,
			blank_tag,
			null_tag,
		};
		struct char_range {
			char_range(char f, char t): from(f), to(t) { }
			const char from, to;
		};
		template <typename Result, typename Arg>
		struct apply_self {
			apply_self(Arg val): val(val) { }
			template <typename T>
			Result operator ()(const T &op) const { return op(val); }
			Arg val;
		};
		struct exists_in: std::unary_function<char, bool> {
			exists_in(): tag(null_tag), set() { }
			exists_in(const class_tag c): tag(c), set() { }
			exists_in(const char c): tag(single_char_tag), set(1, c) { }
			exists_in(const std::string &spec): tag(custom_set_tag), set(spec) { }
			exists_in(const char *const spec): tag(custom_set_tag), set(spec) { }
			exists_in(const char_range &range): tag(custom_set_tag), set(range.to-range.from+1, '\0') {
				for (char c = range.from; c<=range.to; ++c) set[c-range.from] = c;
			}
			exists_in(const char from, const char to): tag(custom_set_tag), set(to-from+1, '\0') {
				for (char c = from; c<=to; ++c) set[c-from] = c;
			}
			bool operator ()(char c) const {
				switch (tag) {
				case single_char_tag: return c==set[0];
				case alnum_tag: return isalnum(c);
				case alpha_tag: return isalpha(c);
				case space_tag: return c==' ' || c=='\t' || c=='\r' || c=='\n';
				case blank_tag: return c==' ' || c=='\t';
				case null_tag: return false;
				case custom_set_tag: ;
				}
				return set.find(c)!=set.npos;
			}
			class_tag tag;
			std::string set;
		};
		struct not_exists_in: std::unary_function<char, bool> {
			typedef exists_in ei;
			typedef std::vector<exists_in> pred_type;
			not_exists_in(ei e1=ei(), ei e2=ei(), ei e3=ei(), ei e4=ei(), ei e5=ei()) {
				pred.push_back(e1);
				pred.push_back(e2);
				pred.push_back(e3);
				pred.push_back(e4);
				pred.push_back(e5);
			}
			bool operator ()(char c) const { return std::find_if(pred.begin(), pred.end(), apply_self<bool, char>(c))==pred.end(); }
			pred_type pred;
		};
		template <typename Iter>
		inline bool skip_spaces(Iter &first, const Iter &last) {
			if (first==last) return false;
			const Iter ite = first;
			first = std::find_if(first, last, not_exists_in(blank_tag));
			return ite!=first;
		}
		template <typename Iter>
		inline bool skip_newline(Iter &first, const Iter &last) {
			if (first==last) return false;
			const Iter ite = first;
			if (*first=='\n') {
				++first;
			} else if (*first==';') {
				first = std::find(first, last, '\n');
				if (first!=last) ++first;
			}
			return ite!=first;
		}
		template <typename Iter>
		inline bool skip_spaces_or_newlines(Iter &first, const Iter &last) {
			if (first==last) return false;
			const Iter ite = first;
			while (skip_spaces(first, last) || skip_newline(first, last)) ;
			return ite!=first;
		}
		template <typename Iter>
		optional<std::string> match_normal_str(Iter &first, const Iter &last) {
			Iter ite = std::find_if(first, last, not_exists_in(alnum_tag, "-_/", char_range(0x80,0xFD)));
			if (ite==last) return none;
			boost::swap(first, ite);
			return std::string(ite, first);
		}
		template <typename Iter>
		optional<std::string> match_value_str(Iter &first, const Iter &last) {
			Iter ite = std::find_if(first, last, not_exists_in(alnum_tag, "-_/.", char_range(0x80,0xFD)));
			if (ite==last) return none;
			boost::swap(first, ite);
			return std::string(ite, first);
		}
		struct quot_terminator: std::binary_function<char, char, bool> {
			bool operator ()(char a, char b) const { return a!='\\' && b=='"'; }
		};
		template <typename Iter>
		optional<std::string> match_quot_str(Iter &first, const Iter &last) {
			if (first==last) return none;
			Iter ite = first;
			skip_spaces(ite, last);
			if (*ite!='"') return none;
			ite = std::adjacent_find(++first, last, quot_terminator());
			if (ite==last) BOOST_THROW_EXCEPTION(git_cfgparser_exception() << errmsg_info("unmatched double-quot"));
			boost::swap(first, ++++ite);
			return std::string(ite, boost::prior(first));
		}
		template <typename Iter>
		bool match_eql_char(Iter &first, const Iter &last) {
			Iter ite = first;
			skip_spaces(ite, last);
			if (ite==last || *ite!='=') return false;
			skip_spaces(++ite, last);
			boost::swap(first, ite);
			return true;
		}
		template <typename Iter>
		optional<value_pair_t> match_value_set(Iter &first, const Iter &last) {
			if (first==last) return none;
			Iter ite = first;
			skip_spaces_or_newlines(ite, last);
			const optional<std::string> name = match_normal_str(ite, last);
			if (!name || !match_eql_char(ite, last)) return none;
			optional<std::string> value = match_quot_str(ite, last);
			if (!value) value = match_value_str(ite, last);
			if (!value) return none;
			boost::swap(first, ite);
			skip_spaces_or_newlines(first, last);
			value_pair_t v = { *name, *value };
			return v;
		}
		template <typename Iter>
		optional<section_header_t> match_sec_header(Iter &first, const Iter &last) {
			Iter ite = first;
			skip_spaces_or_newlines(ite, last);
			if (*ite++!='[') return none;
			const optional<std::string> sec = match_normal_str(ite, last);
			if (!sec) return none;
			optional<std::string> subsec;
			if (*ite=='.') {
				subsec = match_normal_str(++ite, last);
			} else if (*ite==']') {
			} else {
				subsec = match_quot_str(++ite, last);
			}
			++ite;
			boost::swap(first, ite);
			skip_spaces_or_newlines(first, last);
			section_header_t h = { *sec, subsec.get_value_or("") };
			return h;
		}
		template <typename Iter>
		optional<section_t> match_section(Iter &first, const Iter &last) {
			Iter ite = first;
			section_t sec;
			{
				const optional<section_header_t> head = match_sec_header(ite, last);
				if (!head) return none;
				sec.head = *head;
			}
			while (const optional<value_pair_t> val = match_value_set(ite, last)) {
				sec.values.push_back(*val);
			}
			boost::swap(first, ite);
			return sec;
		}
		template <typename Iter>
		vector<section_t> parse_cfg_file(Iter &first, const Iter &last) {
			vector<section_t> vs;
			while (const optional<section_t> sec = match_section(first, last)) {
				vs.push_back(*sec);
			}
			return vs;
		}
	}

	extern const convert_table_type convert_table;
	namespace {
		git_config_value convert_cfg(const std::string &name, const std::string &value) {
			return map_at_or(convert_table, name, default_converter).reader(value);
		}
		void build_config_map(git_config_map &cfg, const std::vector<parser::section_t> &sects) {
			BOOST_FOREACH (const parser::section_t &s, sects) {
				BOOST_FOREACH (const parser::value_pair_t &v, s.values) {
					const git_config_name h(s.head.sec, s.head.sub, v.name);
					if (!h.empty()) cfg.insert(std::make_pair(h, convert_cfg(h.get_key_str(), v.value)));
				}
			}
		}
	}

	git_config_map read_config_file(const ustring &cfgfile) {
		std::string str;
		{
			std::ifstream ifs(cfgfile.a_str().c_str());
			for (std::string s; getline(ifs, s); str.append("\n").append(s)) ;
			std::replace(str.begin(), str.end(), '\r', '\n');
		}
		std::string::iterator first = str.begin();
		std::vector<parser::section_t> sections = parser::parse_cfg_file(first, str.end());
		mgit::git_config_map cfgmap;
		mgit::build_config_map(cfgmap, sections);
		return cfgmap;
	}
}
