#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "git-compat-util.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
	#include BOOST_ABI_PREFIX
#endif

namespace mgit {
	/// Config file type
	enum config_level {
		repository_config,
		user_local_config,
		system_wide_config,
	};
	/// Get config file name.
	MGIT_DECL ustring get_config_file_name(config_level level);

	MGIT_DECL void merge_from_environment();

	struct git_config_name {
		const ustring section;
		const ustring subsection;
		const ustring name;
		const ustring key;
		git_config_name(): section(), subsection(), name(), key() { }
		git_config_name(const ustring &sec, const ustring &sub, const ustring &nam)
			 : section(sec), subsection(sub), name(nam), key(sec+'-'+sub+'-'+nam) { }
		bool empty() const { return section.empty() && subsection.empty() && name.empty(); }
		const std::string get_key_str() const { return section.a_str() + (subsection.empty()?".":".*.") + name.a_str(); }
		friend bool operator ==(const git_config_name &x, const git_config_name &y) { return x.key==y.key; }
		friend bool operator <(const git_config_name &x, const git_config_name &y) { return x.key < y.key; }
	};
	typedef variant<ustring, intmax_t, int, double, bool> git_config_value;
	typedef map<git_config_name, git_config_value> git_config_map;
	MGIT_DECL git_config_map read_config_file(const ustring &cfgfile);
	MGIT_DECL void write_config_file(const ustring &cfgfile, const git_config_map &cfg);
	MGIT_DECL void write_config_file(std::ostream &out, const git_config_map &cfg);
	MGIT_DECL std::ostream &operator <<(std::ostream &os, const git_config_map::value_type &v);
	MGIT_DECL std::ostream &operator <<(std::ostream &os, const git_config_map &v);

	struct config_data_converter {
		git_config_value (*reader)(const std::string &value);
		std::string (*writer)(const git_config_value &);
	};
	MGIT_DECL extern const config_data_converter default_converter;
	typedef map<std::string, config_data_converter> convert_table_type;
}

#ifdef BOOST_HAS_ABI_HEADERS
	#include BOOST_ABI_SUFFIX
#endif

#endif
