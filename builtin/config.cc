#include "git-compat-util.hpp"
#include "builtin.hpp"
#include "config.hpp"

namespace mgit {
	int cmd_config(int argc, char **argv, opt_desc &) {
		opt_desc opt;
		opt.add_options()
			("help,h", "print help")
			("cmd", opt::value<std::string>(), "config command")
			("cfg", opt::value<std::string>(), "config file")
				;
		opt_desc hidden;
		pos_opt_desc pos_opt;
		pos_opt.add("cmd", 1).add("cfg", 1);
		var_map vm = parse_argv(argc, argv, opt, pos_opt);

		if (vm.count("help") || !vm.count("cfg")) return print_usage("git config <cfgfile>");

		std::cout << read_config_file(vm["cfg"].as<std::string>()) << std::endl;
		return 0;
	}

	namespace {
		struct config_symbol { };

		git_config_value convert_cfg_to_string(const std::string &value) { return ustring(value); }
		git_config_value convert_cfg_to_int(const std::string &value) { return lexical_cast<int>(value); }
		git_config_value convert_cfg_to_long(const std::string &value) { return lexical_cast<intmax_t>(value); }
		git_config_value convert_cfg_to_double(const std::string &value) { return lexical_cast<double>(value); }
		git_config_value convert_cfg_to_bool(const std::string &v) {
			//std::string v(value.length(), '\0');
			//std::transform(value.begin(), value.end(), v.begin(), &tolower);
			if (v=="true" || v=="yes"|| v=="y" || v=="on" ) return true;
			if (v=="false"|| v=="no" || v=="n" || v=="off") return false;
			return !!lexical_cast<int>(v);
		}
		//git_config_value convert_cfg_to_symbol(const std::string &value) { return git_config_value(value); }

		const struct convert_table_primitive {
			std::string name;
			git_config_value (*fn)(const std::string &value);
			operator convert_table_type::value_type() const { return std::make_pair(name, fn); }
		} convert_table_def[] = {
#define bool_ &convert_cfg_to_bool
#define int_ &convert_cfg_to_int
#define long_ &convert_cfg_to_long
#define double_ &convert_cfg_to_double
#define string_ &convert_cfg_to_string
#define symbol_ &convert_cfg_to_symbol
			{ "core.fileMode", bool_ },
			{ "core.ignoreCygwinFSTricks", bool_ },
			{ "core.ignorecase", bool_ },
			{ "core.trustctime", bool_ },
			{ "core.quotepath", bool_ },
			{ "core.autocrlf", bool_ },
			{ "core.safecrlf", bool_ },
			{ "core.symlinks", bool_ },
			{ "core.gitProxy", string_ },
			{ "core.ignoreStat", bool_ },
			{ "core.preferSymlinkRefs", bool_ },
			{ "core.bare", bool_ },
			{ "core.worktree", string_ },
			{ "core.logAllRefUpdates", bool_ },
			{ "core.repositoryFormatVersion", double_ },
			{ "core.sharedRepository", string_ },
			{ "core.warnAmbiguousRefs", bool_ },
			{ "core.compression", int_ },
			{ "core.loosecompression", int_ },
			{ "core.packedGitWindowSize", long_ },
			{ "core.packedGitLimit", long_ },
			{ "core.deltaBaseCacheLimit", long_ },
			{ "core.bigFileThreshold", long_ },
			{ "core.excludesfile", string_ },
			{ "core.editor", string_ },
			{ "core.pager", string_ },
			{ "core.whitespace", string_ },
			{ "core.fsyncobjectfiles", bool_ },
			{ "core.preloadindex", bool_ },
			{ "core.createObject", string_ },
			{ "core.notesRef", string_ },
			{ "core.sparseCheckout", bool_ },
#undef symbol_
#undef string_
#undef double_
#undef long_
#undef int_
#undef bool_
		};
	}
	extern const convert_table_type convert_table;
	const convert_table_type convert_table(array_begin(convert_table_def), array_end(convert_table_def));
}
