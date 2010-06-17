#ifndef PATH_HPP_
#define PATH_HPP_

#include "git-compat-util.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
	#include BOOST_ABI_PREFIX
#endif

#ifdef PATH_SEP_BACKSLASH
#define SLASH_STR "\\"
#define SLASH_CHR '\\'
#else
#define SLASH_STR "/"
#define SLASH_CHR '/'
#endif

namespace mgit {
	namespace fsutil {
		MGIT_DECL ustring get_temporary_name(const ustring &where, const ustring &prefix, bool create = false);
		MGIT_DECL bool symlink_usable(const ustring &where);
		MGIT_DECL bool hardlink_usable(const ustring &where);
		MGIT_DECL bool unix_perm_usable(const ustring &where);
		MGIT_DECL bool ignore_case_path(const ustring &where);
		MGIT_DECL ustring add_trailing_slash(const ustring &dir);
		MGIT_DECL ustring del_trailing_slash(const ustring &dir);
		MGIT_DECL ustring remove_file_name(const ustring &fullpath);
		MGIT_DECL ustring extract_file_name(const ustring &fullpath);
		MGIT_DECL ustring get_exec_path();
		MGIT_DECL ustring get_user_config_dir();
	}
}

#ifdef BOOST_HAS_ABI_HEADERS
	#include BOOST_ABI_SUFFIX
#endif

#endif
