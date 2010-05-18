#ifndef PATH_HPP_
#define PATH_HPP_

#include "git-compat-util.hpp"
#include <boost/filesystem.hpp>

namespace mgit {
	namespace fsutil {
		ustring get_temporary_name(const ustring &where, const ustring &prefix, bool create = false);
		bool symlink_usable(const ustring &where);
		bool hardlink_usable(const ustring &where);
		bool unix_perm_usable(const ustring &where);
		bool ignore_case_path(const ustring &where);
		ustring add_trailing_slash(const ustring &dir);
		ustring del_trailing_slash(const ustring &dir);
	}
}

#endif
