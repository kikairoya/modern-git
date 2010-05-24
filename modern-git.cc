#include "git-compat-util.hpp"
#include "builtin.hpp"
#include "environment.hpp"

#include <iostream>

namespace mgit {
	namespace {
		struct printer {
			printer(std::ostream &os): os(os) { }
			void operator ()(const std::string &s) { os << s << '\n'; }
			std::ostream &os;
		};
		struct command_set {
			const char *name;
			cmd_function fn;
		};
		const command_set git_cmds[] = {
			{ "add", &cmd_add },
			{ "am", &cmd_am },
			{ "archive", &cmd_archive },
			{ "bisect", &cmd_bisect },
			{ "branch", &cmd_branch },
			{ "bundle", &cmd_bundle },
			{ "checkout", &cmd_checkout },
			{ "cherry-pick", &cmd_cherry_pick },
			{ "citool", &cmd_citool },
			{ "clean", &cmd_clean },
			{ "colne", &cmd_clone },
			{ "commit", &cmd_commit },
			{ "describe", &cmd_describe },
			{ "diff", &cmd_diff },
			{ "fetch", &cmd_fetch },
			{ "format-patch", &cmd_format_patch },
			{ "gc", &cmd_gc },
			{ "gui", &cmd_gui },
			{ "grep", &cmd_grep },
			{ "init", &cmd_init },
			{ "log", &cmd_log },
			{ "merge", &cmd_merge },
			{ "mv", &cmd_mv },
			{ "notes", &cmd_notes },
			{ "pull", &cmd_pull },
			{ "push", &cmd_push },
			{ "rebase", &cmd_rebase },
			{ "reset", &cmd_reset },
			{ "revert", &cmd_revert },
			{ "rm", &cmd_rm },
			{ "shortlog", &cmd_shortlog },
			{ "show", &cmd_show },
			{ "stash", &cmd_stash },
			{ "status", &cmd_status },
			{ "submodule", &cmd_submodule },
			{ "tag", &cmd_tag },

			{ "config", &cmd_config },
			{ "fast-export", &cmd_fast_export },
			{ "fast-import", &cmd_fast_import },
			{ "filter-branch", &cmd_filter_branch },
			{ "lost-found", &cmd_lost_found },
			{ "mergetool", &cmd_mergetool },
			{ "pack-refs", &cmd_pack_refs },
			{ "prune", &cmd_prune },
			{ "reflog", &cmd_reflog },
			{ "relink", &cmd_relink },
			{ "remote", &cmd_remote },
			{ "repack", &cmd_repack },
			{ "replace", &cmd_replace },
			{ "repo-config", &cmd_repo_config },

			{ "annotate", &cmd_annotate },
			{ "blame", &cmd_blame },
			{ "cherry", &cmd_cherry },
			{ "count-objects", &cmd_count_objects },
			{ "difftool", &cmd_difftool },
			{ "fsck", &cmd_fsck },
			{ "get-tar-commit-id", &cmd_get_tar_commit_id },
			{ "help", &cmd_help },
			{ "instaweb", &cmd_instaweb },
			{ "merge-tree", &cmd_merge_tree },
			{ "rerere", &cmd_rerere },
			{ "rev-parse", &cmd_rev_parse },
			{ "show-branch", &cmd_show_branch },
			{ "verify-tag", &cmd_verify_tag },
			{ "whatchanged", &cmd_whatchanged },

			{ "archimport", &cmd_archimport },
			{ "cvsexportcommit", &cmd_cvsexportcommit },
			{ "cvsimport", &cmd_cvsimport },
			{ "cvsserver", &cmd_cvsserver },
			{ "imap-send", &cmd_imap_send },
			{ "quiltimport", &cmd_quiltimport },
			{ "request-pull", &cmd_request_pull },
			{ "send-email", &cmd_send_email },
			{ "svn", &cmd_svn },

			{ "apply", &cmd_apply },
			{ "checkout-index", &cmd_checkout_index },
			{ "commit-tree", &cmd_commit_tree },
			{ "hash-object", &cmd_hash_object },
			{ "index-pack", &cmd_index_pack },
			{ "merge-file", &cmd_merge_file },
			{ "merge-index", &cmd_merge_index },
			{ "mktag", &cmd_mktag },
			{ "mktree", &cmd_mktree },
			{ "pack-objects", &cmd_pack_objects },
			{ "prune-packed", &cmd_prune_packed },
			{ "read-tree", &cmd_read_tree },
			{ "symbolic-ref", &cmd_symbolic_ref },
			{ "unpack-objects", &cmd_unpack_objects },
			{ "update-index", &cmd_update_index },
			{ "update-ref", &cmd_update_ref },
			{ "write-tree", &cmd_write_tree },

			{ "cat-file", &cmd_cat_file },
			{ "diff-files", &cmd_diff_files },
			{ "diff-index", &cmd_diff_index },
			{ "diff-tree", &cmd_diff_tree },
			{ "for-each-ref", &cmd_for_each_ref },
			{ "ls-files", &cmd_ls_files },
			{ "ls-remove", &cmd_ls_remote },
			{ "merge-base", &cmd_merge_base },
			{ "name-rev", &cmd_name_rev },
			{ "pack-redundant", &cmd_pack_redundant },
			{ "rev-list", &cmd_rev_list },
			{ "show-index", &cmd_show_index },
			{ "show-ref", &cmd_show_ref },
			{ "tar-tree", &cmd_tar_tree },
			{ "unpack-file", &cmd_unpack_file },
			{ "var", &cmd_var },
			{ "verify-pack", &cmd_verify_pack },

			{ "daemon", &cmd_daemon },
			{ "fetch-pack", &cmd_fetch_pack },
			{ "http-backend", &cmd_http_backend },
			{ "send-pack", &cmd_send_pack },
			{ "update-server-info", &cmd_update_server_info },

			{ "http-fetch", &cmd_http_fetch },
			{ "http-push", &cmd_http_push },
			{ "parse-remote", &cmd_parse_remote },
			{ "receive-pack", &cmd_receive_pack },
			{ "shell", &cmd_shell },
			{ "upload-archive", &cmd_upload_archive },
			{ "upload-pack", &cmd_upload_pack },

			{ "check-attr", &cmd_check_attr },
			{ "check-ref-format", &cmd_check_ref_format },
			{ "fmt-merge-msg", &cmd_fmt_merge_msg },
			{ "mailinfo", &cmd_mailinfo },
			{ "mailsplit", &cmd_mailsplit },
			{ "merge-one-file", &cmd_merge_one_file },
			{ "patch-id", &cmd_patch_id },
			{ "peek-remote", &cmd_peek_remote },
			{ "sh-setup", &cmd_sh_setup },
			{ "stripspace", &cmd_stripspace },
		};
		struct match_command: std::unary_function<const command_set &, bool> {
			match_command(const std::string &arg): arg(arg) { }
			bool operator ()(const command_set &cmd) { return arg == cmd.name; }
			std::string arg;
		};
	}
	int main(int argc, char **argv) {
		opt_desc general_option("General Option");
		general_option.add_options()
			("version", "print version information")
			("help", "print help message")
			("exec-path", opt::value<std::string>(), "specify exec path or print default path")
			("html-path", "print html path")
			("pagenate,p", opt::value<std::string>(), "specify pager")
			("no-pager", "not use pager")
			("no-replace-objects", "see git-replace(1)")
			("bare", "treat as bare repository")
			("git-dir", opt::value<std::string>(), "specify git repository")
			("work-tree", opt::value<std::string>(), "specify working-tree directory")
				;
		opt_desc hidden_option("Hidden");
		hidden_option.add_options()
			("command", opt::value<std::string>(), "command to run")
			("targets", opt::value< vector<std::string> >(), "target files, revs, tags, branches, repos...")
				;
		general_option.add(hidden_option);
		pos_opt_desc nonhyphen_option;
		nonhyphen_option.add("command", 1).add("targets", -1);

		var_map vm = parse_argv(argc, argv, general_option, nonhyphen_option);

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
		if (!vm.count("command")) {
			std::cout << "no command specified" << std::endl;
			return 0;
		}
		if (vm.count("exec-path")) override_env(GIT_EXEC_PATH, vm["exec-path"].as<std::string>());
		if (vm.count("git-dir")) override_env(GIT_DIR, vm["git-dir"].as<std::string>());
		if (vm.count("work-tree")) override_env(GIT_WORK_TREE, vm["work-tree"].as<std::string>());
		if (vm.count("paginate")) override_env(GIT_PAGER, vm["paginate"].as<std::string>());

		const std::string cmdstr = vm["command"].as<std::string>();
		const command_set *pcmd = std::find_if(begin(git_cmds), end(git_cmds), match_command(cmdstr));
		if (pcmd==end(git_cmds)) {
			std::cout << "no such command: " << cmdstr << std::endl;
			return 1;
		}
		return pcmd->fn(argc, argv, general_option);
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
