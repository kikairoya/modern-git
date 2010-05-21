#include "git-compat-util.hpp"
#include "path.hpp"

#include "environment.hpp"

#include <string.h>

namespace mgit {

#define GITATTRIBUTES_FILE ".gitattributes"
#define INFOATTRIBUTES_FILE "info/attributes"
#define ATTRIBUTE_MACRO_PREFIX "[attr]"
#define GIT_NOTES_REF_ENVIRONMENT "GIT_NOTES_REF"
#define GIT_NOTES_DEFAULT_REF "refs/notes/commits"
#define GIT_NOTES_DISPLAY_REF_ENVIRONMENT "GIT_NOTES_DISPLAY_REF"
#define GIT_NOTES_REWRITE_REF_ENVIRONMENT "GIT_NOTES_REWRITE_REF"
#define GIT_NOTES_REWRITE_MODE_ENVIRONMENT "GIT_NOTES_REWRITE_MODE"

	typedef map<std::string, ustring> env_type;
	namespace {
		template <typename MapT>
		inline std::pair<typename MapT::iterator, bool> map_insert(MapT &m, const typename MapT::key_type &k, const typename MapT::mapped_type &v) {
			return m.insert(std::make_pair(k, v));
		}
		void fill_from_config_file(env_type &e) {
			// STUB
		}
		void fill_default_environ(env_type &e) {
			map_insert(e, GIT_DIR, ".git");
			map_insert(e, GIT_DB_DIR, e[GIT_DIR] + "/objects");
			map_insert(e, GIT_REFS_DIR, e[GIT_DIR] + "/refs");
			map_insert(e, GIT_INDEX_FILE, e[GIT_DIR] + "/index");
			map_insert(e, GIT_GRAFT_FILE, e[GIT_DIR] + "/info/grafts");
		}
		env_type init_git_environ() {
			env_type e;
			char **envp = environ;
			do {
				if (const char *p = strchr(*envp, '=')) {
					map_insert(e, std::string(*envp, p-*envp), ustring(p+1));
				} else {
					e[*envp];
				}
			} while (*++envp);
			fill_from_config_file(e);
			fill_default_environ(e);
			return e;
		}
		env_type git_environ(init_git_environ()); // file-global var.
	}

	ustring query_env(const std::string &name) {
		env_type::const_iterator ite = git_environ.find(name);
		if (ite == git_environ.end()) throw std::runtime_error("cannot find environment variable " + name);
		return ite->second;
	}

	ustring query_env(const std::string &name, const ustring &defval) {
		env_type::const_iterator ite = git_environ.find(name);
		return ite == git_environ.end() ? defval : ite->second;
	}

	void override_env(const std::string &name, const ustring &newval) {
		git_environ[name] = newval;
	}
}
