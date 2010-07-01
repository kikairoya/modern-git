#include "mgit-export.hpp"
#include <exception>
#include <iostream>

namespace mgit {
	MGIT_DECL int main(int, char **);
}

int main(int argc, char **argv) {
	try {
		return mgit::main(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 127;
	}
}
