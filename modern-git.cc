#include "git-compat-util.hpp"
#include "builtin.hpp"

#include <iostream>
#include <boost/program_options.hpp>

namespace mgit {
	namespace {
		struct printer {
			printer(std::ostream &os): os(os) { }
			void operator ()(const ustring &s) { os << s.a_str() << '\n'; }
			std::ostream &os;
		};
	}
	int main(vector<ustring> args) {
		std::for_each(args.begin(), args.end(), printer(std::cout));
		return 0;
	}
}

int main(int argc, char **argv) {
	std::vector<mgit::ustring> a(argc);
	std::copy(argv, argv+argc, a.begin());
	return mgit::main(a);
}
