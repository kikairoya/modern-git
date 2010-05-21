#include "git-compat-util.hpp"
#include "builtin.hpp"
#include "environment.hpp"

#include <iostream>
#include <boost/program_options.hpp>

namespace mgit {
	namespace {
		namespace po = boost::program_options;
		struct printer {
			printer(std::ostream &os): os(os) { }
			void operator ()(const std::string &s) { os << s << '\n'; }
			std::ostream &os;
		};
	}
	int main(int argc, char **argv) {
		using namespace po;
		options_description general_option("General Option");
		general_option.add_options()
			("version", "print version information")
			("help", "print help message")
			("exec-path", value<std::string>()->default_value(""), "specify exec path or print default path")
			("html-path", "print html path")
			("pagenate,p", value<std::string>(), "specify pager")
			("no-pager", "not use pager")
			("no-replace-objects", "see git-replace(1)")
			("bare", "treat as bare repository")
			("git-dir", value<std::string>(), "specify git repository")
			("work-tree", value<std::string>(), "specify working-tree directory")
				;
		options_description hidden_option("Hidden");
		hidden_option.add_options()
			("command", value<std::string>(), "command to run")
			("targets", value< vector<std::string> >(), "target files, revs, tags, branches, repos...")
				;
		general_option.add(hidden_option);
		positional_options_description nonhyphen_option;
		nonhyphen_option.add("command", 1).add("targets", -1);
		variables_map vm;
		store(command_line_parser(argc, argv).options(general_option).positional(nonhyphen_option).run(), vm);

		bool no_op = false;

		if (vm.count("help")) {
			std::cout <<
				"usage: git [--version] [--exec-path[=GIT_EXEC_PATH]] [--html-path]\n"
				"           [-p|--paginate|--no-pager] [--no-replace-objects]\n"
				"           [--bare] [--git-dir=GIT_DIR] [--work-tree=GIT_WORK_TREE]\n"
				"           [--help] COMMAND [ARGS]\n"
				"\n"
				"The most commonly used git commands are:\n"
				"   add        Add file contents to the index\n"
				"   bisect     Find by binary search the change that introduced a bug\n"
				"   branch     List, create, or delete branches\n"
				"   checkout   Checkout a branch or paths to the working tree\n"
				"   clone      Clone a repository into a new directory\n"
				"   commit     Record changes to the repository\n"
				"   diff       Show changes between commits, commit and working tree, etc\n"
				"   fetch      Download objects and refs from another repository\n"
				"   grep       Print lines matching a pattern\n"
				"   init       Create an empty git repository or reinitialize an existing one\n"
				"   log        Show commit logs\n"
				"   merge      Join two or more development histories together\n"
				"   mv         Move or rename a file, a directory, or a symlink\n"
				"   pull       Fetch from and merge with another repository or a local branch\n"
				"   push       Update remote refs along with associated objects\n"
				"   rebase     Forward-port local commits to the updated upstream head\n"
				"   reset      Reset current HEAD to the specified state\n"
				"   rm         Remove files from the working tree and from the index\n"
				"   show       Show various types of objects\n"
				"   status     Show the working tree status\n"
				"   tag        Create, list, delete or verify a tag object signed with GPG\n"
				"\n"
				"See 'git help COMMAND' for more information on a specific command.\n"
					<< std::flush;
			no_op = true;
		}
		if (vm.count("version")) {
			std::cout << "modern-git under-construction" << std::endl;
			no_op = true;
		}
		if (vm.count("exec-path") && vm["exec-path"].as<std::string>().empty()) {
			no_op = true;
		}
		if (vm.count("html-path")) {
			no_op = true;
		}
		if (no_op) return 0;
		if (vm.count("exec-path")) override_env(GIT_EXEC_PATH, vm["exec-path"].as<std::string>());
		if (vm.count("git-dir")) override_env(GIT_DIR, vm["git-dir"].as<std::string>());
		if (vm.count("work-tree")) override_env(GIT_WORK_TREE, vm["work-tree"].as<std::string>());
		std::for_each(argv, argv+argc, printer(std::cout));
		return 0;
	}
}

int main(int argc, char **argv) {
	try {
		return mgit::main(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 127;
	}
}
