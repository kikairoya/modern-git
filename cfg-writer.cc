#include "git-compat-util.hpp"
#include "config.hpp"
#include <boost/foreach.hpp>
#include <fstream>

namespace mgit {
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
		std::string convert_cfg(const git_config_name &name, const git_config_value &value) {
			return map_at_or(convert_table, name.get_key_str(), default_converter).writer(value);
		}
	}
	void write_config_file(const ustring &cfgfile, const git_config_map &cfg) {
		std::ofstream out(cfgfile.a_str().c_str());
		write_config_file(out, cfg);
	}
	void write_config_file(std::ostream &out, const git_config_map &cfg) {
		std::pair<ustring, ustring> prev_sect;
		BOOST_FOREACH (git_config_map::value_type vv, cfg) {
			if (!vv.first.empty()) {
				if (vv.first.section!=prev_sect.first || vv.first.subsection!=prev_sect.second) {
					out << '[' << vv.first.section;
					if (!vv.first.subsection.empty()) out << '.' << vv.first.subsection;
					out << ']' << '\n';
				}
				out << '\t' << vv.first.name << " = " << convert_cfg(vv.first, vv.second) << '\n';
				prev_sect.first = vv.first.section;
				prev_sect.second = vv.first.subsection;
			}
		}
	}
}
