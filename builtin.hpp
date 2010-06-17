#ifndef BUILTIN_HPP_
#define BUILTIN_HPP_

#include "mgit-export.hpp"
#include <iostream>
#include <boost/program_options.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
	#include BOOST_ABI_PREFIX
#endif

namespace mgit {
	typedef boost::program_options::options_description opt_desc;
	typedef boost::program_options::positional_options_description pos_opt_desc;
	typedef boost::program_options::variables_map var_map;
	namespace opt {
		using boost::program_options::value;
	}

	typedef int (*cmd_function)(int argc, char **argv, opt_desc &);
	inline var_map parse_argv(int argc, char **argv, opt_desc &opt, pos_opt_desc &cmd) {
		using namespace boost::program_options;
		var_map vm;
		store(command_line_parser(argc, argv).options(opt).positional(cmd).run(), vm);
		return vm;
	}

	inline int print_usage(const std::string &s) {
		std::cout << s << std::endl;
		return 0;
	}

	inline int cmd_add(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_am(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_archive(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_bisect(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_branch(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_bundle(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_checkout(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_cherry_pick(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_citool(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_clean(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_clone(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_commit(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_describe(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_diff(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_fetch(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_format_patch(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_gc(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_gui(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_grep(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_init(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_log(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_merge(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_mv(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_notes(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_pull(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_push(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_rebase(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_reset(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_revert(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_rm(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_shortlog(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_show(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_stash(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_status(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_submodule(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_tag(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	MGIT_DECL int cmd_config(int argc, char **argv, opt_desc &opt);
	inline int cmd_fast_export(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_fast_import(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_filter_branch(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_lost_found(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_mergetool(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_pack_refs(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_prune(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_reflog(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_relink(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_remote(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_repack(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_replace(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_repo_config(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	inline int cmd_annotate(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_blame(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_cherry(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_count_objects(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_difftool(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_fsck(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_get_tar_commit_id(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_help(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_instaweb(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_merge_tree(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_rerere(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_rev_parse(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_show_branch(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_verify_tag(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_whatchanged(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	inline int cmd_archimport(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_cvsexportcommit(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_cvsimport(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_cvsserver(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_imap_send(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_quiltimport(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_request_pull(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_send_email(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_svn(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	inline int cmd_apply(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_checkout_index(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_commit_tree(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_hash_object(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_index_pack(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_merge_file(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_merge_index(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_mktag(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_mktree(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_pack_objects(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_prune_packed(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_read_tree(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_symbolic_ref(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_unpack_objects(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_update_index(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_update_ref(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_write_tree(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	inline int cmd_cat_file(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_diff_files(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_diff_index(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_diff_tree(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_for_each_ref(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_ls_files(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_ls_remote(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_merge_base(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_name_rev(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_pack_redundant(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_rev_list(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_show_index(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_show_ref(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_tar_tree(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	MGIT_DECL int cmd_unpack_file(int argc, char **argv, opt_desc &opt);
	inline int cmd_var(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_verify_pack(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	inline int cmd_daemon(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_fetch_pack(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_http_backend(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_send_pack(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_update_server_info(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	inline int cmd_http_fetch(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_http_push(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_parse_remote(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_receive_pack(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_shell(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_upload_archive(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_upload_pack(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }

	inline int cmd_check_attr(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_check_ref_format(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_fmt_merge_msg(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_mailinfo(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_mailsplit(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_merge_one_file(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_patch_id(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_peek_remote(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_sh_setup(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
	inline int cmd_stripspace(int argc, char **argv, opt_desc &opt) { std::cout << "stub" << std::endl; return 1; }
}

#ifdef BOOST_HAS_ABI_HEADERS
	#include BOOST_ABI_SUFFIX
#endif

#endif
