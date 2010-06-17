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
	ustring get_config_file_name(config_level level);

	void merge_from_environment();

	struct git_config_name {
		const ustring section;
		const ustring subsection;
		const ustring name;
		const ustring key;
		git_config_name(): section(), subsection(), name(), key() { }
		git_config_name(const ustring &sec, const ustring &sub, const ustring &nam)
			 : section(sec), subsection(sub), name(nam), key(sec+'.'+sub+'.'+nam) { }
		friend bool operator ==(const git_config_name &x, const git_config_name &y) { return x.key==y.key; }
		friend bool operator <(const git_config_name &x, const git_config_name &y) { return x.key < y.key; }
	};
	typedef variant<ustring, intmax_t, int, double, bool> git_config_value;
	typedef map<git_config_name, git_config_value> git_config_map;
	MGIT_DECL git_config_map read_config_file(const ustring &cfgfile);
	std::ostream &operator <<(std::ostream &os, const git_config_map::value_type &v);
	std::ostream &operator <<(std::ostream &os, const git_config_map &v);

	typedef git_config_value (*convert_fn)(const std::string &value);
	typedef map<std::string, convert_fn> convert_table_type;
}

#ifdef BOOST_HAS_ABI_HEADERS
	#include BOOST_ABI_SUFFIX
#endif

#endif
