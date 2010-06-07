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
	
	struct config_symbol { };

	git_config_value convert_cfg_to_string(const std::string &value) { return value; }
	git_config_value convert_cfg_to_int(const std::string &value) { return boost::lexical_cast<long long>(value); }
	git_config_value convert_cfg_to_bool(const std::string &value) {
		std::string v(value.length(), '\0');
		std::transform(value.begin(), value.end(), v.begin(), &tolower);
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
		{ "core.str", &convert_cfg_to_string },
		{ "core.num", &convert_cfg_to_int },
	};
	extern const convert_table_type convert_table(array_begin(convert_table_def), array_end(convert_table_def));
}
