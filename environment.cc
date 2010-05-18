#include "git-compat-util.hpp"
#include "path.hpp"

namespace mgit {

#define GIT_DIR_ENVIRONMENT "GIT_DIR"
#define GIT_WORK_TREE_ENVIRONMENT "GIT_WORK_TREE"
#define DEFAULT_GIT_DIR_ENVIRONMENT ".git"
#define DB_ENVIRONMENT "GIT_OBJECT_DIRECTORY"
#define INDEX_ENVIRONMENT "GIT_INDEX_FILE"
#define GRAFT_ENVIRONMENT "GIT_GRAFT_FILE"
#define TEMPLATE_DIR_ENVIRONMENT "GIT_TEMPLATE_DIR"
#define CONFIG_ENVIRONMENT "GIT_CONFIG"
#define EXEC_PATH_ENVIRONMENT "GIT_EXEC_PATH"
#define CEILING_DIRECTORIES_ENVIRONMENT "GIT_CEILING_DIRECTORIES"
#define NO_REPLACE_OBJECTS_ENVIRONMENT "GIT_NO_REPLACE_OBJECTS"
#define GITATTRIBUTES_FILE ".gitattributes"
#define INFOATTRIBUTES_FILE "info/attributes"
#define ATTRIBUTE_MACRO_PREFIX "[attr]"
#define GIT_NOTES_REF_ENVIRONMENT "GIT_NOTES_REF"
#define GIT_NOTES_DEFAULT_REF "refs/notes/commits"
#define GIT_NOTES_DISPLAY_REF_ENVIRONMENT "GIT_NOTES_DISPLAY_REF"
#define GIT_NOTES_REWRITE_REF_ENVIRONMENT "GIT_NOTES_REWRITE_REF"
#define GIT_NOTES_REWRITE_MODE_ENVIRONMENT "GIT_NOTES_REWRITE_MODE"

	map<std::string, ustring> git_environ;
	const std::string git_dir = "git_dir";
	const std::string git_object_dir = "git_object_dir";
	const std::string git_refs_dir = "git_refs_dir";
	const std::string git_index_file = "git_index_file";
	const std::string git_graft_dir = "git_graft_dir";

	struct git_env_initializer {
		git_env_initializer() {
			{
				ustring s = getenv(GIT_DIR_ENVIRONMENT);
				//if (s.empty()) s = read_gitfile_gently(DEFAULT_GIT_DIR_ENVIRONMENT);
				if (s.empty()) s = DEFAULT_GIT_DIR_ENVIRONMENT;
				git_environ[git_dir] = fsutil::del_trailing_slash(s);
			} {
				ustring s = getenv(DB_ENVIRONMENT);
				if (s.empty()) s = git_environ[git_dir] + "/objects";
				git_environ[git_object_dir] = fsutil::del_trailing_slash(s);
			} {
				git_environ[git_refs_dir] = git_environ[git_dir] + "/refs";
			} {
				ustring s = getenv(INDEX_ENVIRONMENT);
				if (s.empty()) s = git_environ[git_dir] + "/index";
				git_environ[git_index_file] = fsutil::del_trailing_slash(s);
			} {
				ustring s = getenv(GRAFT_ENVIRONMENT);
				if (s.empty()) s = git_environ[git_dir] + "/info/grafts";
				git_environ[git_graft_dir] = fsutil::del_trailing_slash(s);
			}
		}
	} init;
	ustring get_git_dir() {
		return git_environ[git_dir];
	}
}
