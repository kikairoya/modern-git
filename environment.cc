#include "git-compat-util.hpp"
#include "path.hpp"
#include "config.hpp"
#include "environment.hpp"
#include "path.hpp"

#include <string.h>
#include <stdexcept>
#include <assert.h>

// copy from Hamigaki.Process
#if defined(__APPLE__) && defined(__DYNAMIC__)
    #include <crt_externs.h>
    #if !defined(environ)
        #define environ (*_NSGetEnviron())
    #endif
#else
#ifndef _MSC_VER
extern "C"
{
    extern char** environ;
}
#endif
#endif

namespace mgit {

#define GITATTRIBUTES_FILE ".gitattributes"
#define INFOATTRIBUTES_FILE "info/attributes"
#define ATTRIBUTE_MACRO_PREFIX "[attr]"
#define GIT_NOTES_REF_ENVIRONMENT "GIT_NOTES_REF"
#define GIT_NOTES_DEFAULT_REF "refs/notes/commits"
#define GIT_NOTES_DISPLAY_REF_ENVIRONMENT "GIT_NOTES_DISPLAY_REF"
#define GIT_NOTES_REWRITE_REF_ENVIRONMENT "GIT_NOTES_REWRITE_REF"
#define GIT_NOTES_REWRITE_MODE_ENVIRONMENT "GIT_NOTES_REWRITE_MODE"

	namespace {
		ustring get_repository_config_file_name() {
			static const ustring fname = getenv(GIT_DIR) ? fsutil::add_trailing_slash(getenv(GIT_DIR)) + "config" : ".git/config" ;
			return fname;
		}

		ustring get_user_local_config_file_name() {
			return fsutil::add_trailing_slash(fsutil::get_user_config_dir()) + ".gitconfig";
		}

		ustring get_system_wide_config_file_name() {
			using namespace fsutil;
			static const ustring fname = add_trailing_slash(remove_file_name(remove_file_name(get_exec_path()))) + "etc/gitconfig";
			return fname;
		}
	}
	ustring get_config_file_name(config_level level) {
		switch (level) {
		case repository_config: return get_repository_config_file_name();
		case user_local_config: return get_user_local_config_file_name();
		case system_wide_config:return get_system_wide_config_file_name();
		}
		assert(!"unreachable");
	}

	typedef map<std::string, ustring> env_type;
	namespace {
		template <typename MapT>
		inline bool map_exists(const MapT &m, typename MapT::key_type &k) {
			return m.find(k) != m.end();
		}
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
			map_insert(e, GIT_EXEC_PATH, fsutil::remove_file_name(fsutil::get_exec_path()));
		}
		env_type git_environ; // file-global var.
	}

	void init_env() {
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
		std::swap(git_environ, e);
	}

	ustring query_env(const std::string &name) {
		assert(!git_environ.empty());
		env_type::const_iterator ite = git_environ.find(name);
		if (ite == git_environ.end()) throw std::runtime_error("cannot find environment variable " + name);
		return ite->second;
	}

	ustring query_env(const std::string &name, const ustring &defval) {
		assert(!git_environ.empty());
		env_type::const_iterator ite = git_environ.find(name);
		return ite == git_environ.end() ? defval : ite->second;
	}

	void override_env(const std::string &name, const ustring &newval) {
		assert(!git_environ.empty());
		git_environ[name] = newval;
	}
}
