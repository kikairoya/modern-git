#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>

namespace mgit {
	namespace fsutil {
		namespace {
			inline char ntoc(unsigned n) { return n<10 ? n+'a' : 'z'-n; }
			inline ustring nval_to_str(unsigned v) {
				char buf[5];
				buf[4] = 0;
				buf[3] = ntoc(v%36); v /= 36;
				buf[2] = ntoc(v%36); v /= 36;
				buf[1] = ntoc(v%36); v /= 36;
				buf[0] = ntoc(v%36); v /= 36;
				return ustring(buf, enc_utf_8);
			}
		}
		ustring get_temporary_name(const ustring &where, const ustring &prefix, bool create) {
			for (unsigned n=0; n<36*36*36*36; ++n) {
				const ustring f = fsutil::add_trailing_slash(where) + prefix + nval_to_str(n) + ustring(".tmp", enc_utf_8);
				const int fd = open(f.a_str().c_str(), O_RDWR | (create?O_CREAT:0));
				if (fd>0) {
					close(fd);
					return f;
				}
			}
			throw std::runtime_error("cannot create temporary file name at " + where.a_str() + '\\' + prefix.a_str() + "XXXX.TMP");
		}
		bool symlink_usable(const ustring &where) {
			const ustring trgname = get_temporary_name(where, "tht", true);
			const ustring lnkname = get_temporary_name(where, "thl");
			if (!symlink(trgname.a_str().c_str(), lnkname.a_str().c_str())) {
				unlink(lnkname.a_str().c_str());
				unlink(trgname.a_str().c_str());
				return true;
			}
			return false;
		}
		bool hardlink_usable(const ustring &where) {
			const ustring trgname = get_temporary_name(where, "tht", true);
			const ustring lnkname = get_temporary_name(where, "thl");
			if (!link(trgname.a_str().c_str(), lnkname.a_str().c_str())) {
				unlink(lnkname.a_str().c_str());
				unlink(trgname.a_str().c_str());
				return true;
			}
			return false;
		}
		bool unix_perm_usable(const ustring &) { return false; }
		bool ignore_case_path(const ustring &) { return false; }

		ustring get_user_config_dir() {
			static const ustring dir = del_trailing_slash(getenv("HOME") ? getenv("HOME") : getpwuid(getuid())->pw_dir);
			return dir;
		}

	}
}
