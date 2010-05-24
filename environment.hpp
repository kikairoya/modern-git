#ifndef ENVIRONMENT_HPP_
#define ENVIRONMENT_HPP_

#include "git-compat-util.hpp"

namespace mgit {

#define GIT_INDEX_FILE "GIT_INDEX_FILE"
#define GIT_OBJ_DIR "GIT_OBJECT_DIRECTORY"
#define GIT_ALT_OBJ_DIRS "GIT_ALTERNATE_OBJECT_DIRECTORIES"
#define GIT_DIR "GIT_DIR"
#define GIT_WORK_TREE "GIT_WORK_TREE"
#define GIT_CEILING_DIRS "GIT_CEILING_DIRECTORIES"

#define GIT_AUTHOR_NAME "GIT_AUTHOR_NAME"
#define GIT_AUTHOR_EMAIL "GIT_AUTHOR_EMAIL"
#define GIT_COMMITTER_NAME "GIT_COMMITTER_NAME"
#define GIT_COMMITTER_EMAIL "GIT_COMMITTER_EMAIL"
#define GIT_COMMITTER_DATE "GIT_COMMITTER_DATE"
#define EMAIL "EMAIL"

#define GIT_DIFF_OPTS "GIT_DIFF_OPTS"
#define GIT_EXTERNAL_DIFF "GIT_EXTERNAL_DIFF"

#define GIT_MERGE_VERBOSITY "GIT_MERGE_VERBOSITY"
#define GIT_PAGER "GIT_PAGER"
#define GIT_SSH "GIT_SSH"
#define GIT_FLUSH "GIT_FLUSH"
#define GIT_TRACE "GIT_TRACE"

#define GIT_DB_DIR "GIT_OBJECT_DIRECTORY"
#define GIT_REFS_DIR "GIT_REFS_DIRECTORY"
#define GIT_GRAFT_FILE "GIT_GRAFT_FILE"
#define GIT_TEMPLATE_DIR "GIT_TEMPLATE_DIR"
#define GIT_CONFIG "GIT_CONFIG"
#define GIT_EXEC_PATH "GIT_EXEC_PATH"
#define GIT_NO_REPLACE_OBJECTS "GIT_NO_REPLACE_OBJECTS"

	/// Get value from environment or config file. If value is not found, throw runtime_error.
	ustring query_env(const std::string &name);
	/// Get value from environment or config file with default-value.
	ustring query_env(const std::string &name, const ustring &defval);
	/// Set environment value.
	void override_env(const std::string &name, const ustring &newval);

	/// Config file type
	enum config_level {
		repository_config,
		user_local_config,
		system_wide_config,
	};
	/// Get config file name.
	ustring get_config_file_name(config_level level);
}

#endif
