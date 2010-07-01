#include "git-compat-util.hpp"
#include "builtin.hpp"

namespace mgit {
	int cmd_unpack_file(int argc, char **argv, opt_desc &) {
		opt_desc opt;
		opt.add_options()
			("help,h", "print help")
			("cmd", opt::value<std::string>(), "unpack-file command")
			("sha1", opt::value<std::string>(), "target hash")
				;
		opt_desc hidden;
		pos_opt_desc pos_opt;
		pos_opt.add("cmd", 1).add("sha1", 1);
		var_map vm = parse_argv(argc, argv, opt, pos_opt);

		if (vm.count("help") || !vm.count("sha1")) return print_usage("git unpack-file <sha1>");

		std::cout << "unpack file '" << vm["sha1"].as<std::string>() << "'" << std::endl;
		return 0;
	}
}
