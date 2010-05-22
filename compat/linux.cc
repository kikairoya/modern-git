#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>

#include <unistd.h>

namespace mgit {
	namespace fsutil {
		ustring get_exec_path() {
			size_t len = MAX_PATH;
			while (1) {
				vararray<char> buf(len);
				int ret = readlink("/proc/self/exe", buf.c_array(), buf.size());
				if (ret<0) throw std::runtime_error("couldn'd get exec-path");
				if (ret<buf.size()) return buf.c_array();
				len *= 2;
			}
		}
	}
}
