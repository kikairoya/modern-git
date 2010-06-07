#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>

#include <sys/types.h>
#include <sys/sysctl.h>

namespace mgit {
	namespace fsutil {
		ustring get_exec_path() {
			int name[] = {
				CTL_KERN,
				KERN_PROC,
				KERN_PROC_PATHNAME,
				-1
			};
			static const u_int namelen = sizeof(name)/sizeof(name[0]);
			size_t oldlen = 0;
			if (sysctl(name, namelen, 0, &oldlen, 0, 0) == -1) {
				throw std::runtime_error("couldn'd get exec-path");
			}
			if (oldlen == 0) {
				return ustring();
			}
			vararray<char> buf(oldlen);
			if (sysctl(name, namelen, &buf[0], &oldlen, 0, 0) == -1) {
				throw std::runtime_error("couldn'd get exec-path");
			}
			return ustring(&buf[0]);
		}
	}
}
