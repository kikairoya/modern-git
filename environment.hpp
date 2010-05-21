#ifndef ENVIRONMENT_HPP_
#define ENVIRONMENT_HPP_

#include "git-compat-util.hpp"

namespace mgit {
#define GIT_DIR "GIT_DIR"
#define GIT_WORK_TREE "GIT_WORK_TREE"
#define GIT_DB_DIR "GIT_OBJECT_DIRECTORY"
#define GIT_REFS_DIR "GIT_REFS_DIRECTORY"
#define GIT_INDEX_FILE "GIT_INDEX_FILE"
#define GIT_GRAFT_FILE "GIT_GRAFT_FILE"
#define GIT_TEMPLATE_DIR "GIT_TEMPLATE_DIR"
#define GIT_CONFIG "GIT_CONFIG"
#define GIT_EXEC_PATH "GIT_EXEC_PATH"
#define GIT_CEILING_DIRECTORIES "GIT_CEILING_DIRECTORIES"
#define GIT_NO_REPLACE_OBJECTS "GIT_NO_REPLACE_OBJECTS"

	/// Get value from environment or config file. If value is not found, throw runtime_error.
	ustring query_env(const std::string &name);
	/// Get value from environment or config file with default-value.
	ustring query_env(const std::string &name, const ustring &defval);
	/// Set environment value.
	void override_env(const std::string &name, const ustring &newval);
}

#endif
