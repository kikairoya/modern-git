#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>

namespace mgit {
	namespace fsutil {
		ustring add_trailing_slash(const ustring &dir) {
			const ustring tmp = del_trailing_slash(dir);
			if (tmp.length()==1 && ( *tmp.u_str()=='/' || *tmp.u_str()=='\\')) return tmp;
			return tmp + ustring(SLASH_STR, enc_utf_8);
		}

		ustring del_trailing_slash(const ustring &dir) {
			if (dir.length()<=1) return dir;
			switch (*dir.crbegin()) {
			case '/':
			case '\\':
				return del_trailing_slash(ustring(std::string(dir.u_str(), 0, dir.length()-1), enc_utf_8));
			}
			return dir;
		}

		ustring remove_file_name(const ustring &path) {
			if (path.empty()) return ustring();
			const std::string tmp = del_trailing_slash(path).u_str();
			if (tmp=="/" || tmp=="\\") return ustring(SLASH_STR, enc_utf_8);
			size_t pos = tmp.find_last_of("\\/");
			if (pos==std::string::npos) return ustring(".", enc_utf_8);
			if (pos==0) return ustring(SLASH_STR, enc_utf_8);
			return ustring(tmp.substr(0, pos), enc_utf_8);
		}
		
		ustring extract_file_name(const ustring &path) {
			if (path.empty()) return ustring();
			const std::string tmp = del_trailing_slash(path).u_str();
			if (tmp=="/" || tmp=="\\") return ustring(SLASH_STR, enc_utf_8);
			size_t pos = tmp.find_last_of("\\/");
			if (pos==std::string::npos) return path;
			return ustring(tmp.substr(pos+1), enc_utf_8);
		}
	}
}
