#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>

#include <stdlib.h>

namespace mgit {
	namespace fsutil {
		ustring get_exec_path() {
			const char* name = ::getexecname();
			if (!name) throw std::runtime_error("couldn'd get exec-path");
			return ustring(name);
		}
	}
}
