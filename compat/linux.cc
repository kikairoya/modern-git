#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>
#include <stdlib.h>
#include <langinfo.h>
#include <fcntl.h>
#include <iconv.h>
#include <locale.h>

namespace mgit {

	namespace {
		__attribute__((constructor)) void loc_init() {
			setlocale(LC_ALL, "");
		}
		inline std::string get_host_enc_str() {
			const char *s = nl_langinfo(CODESET);
			const std::string en(s ? s : "UTF-8");
			if (en=="CP720") return "WINDOWS-1256";
			return en;
		}
		struct auto_iconv_t {
			auto_iconv_t(const char *from, const char *to): ic(iconv_open(from, to)) { }
			~auto_iconv_t() { iconv_close(ic); }
			operator iconv_t () const { return ic; }
		private:
			auto_iconv_t(const auto_iconv_t &);
			auto_iconv_t &operator =(const auto_iconv_t &);
			iconv_t ic;
		};

		std::string do_conv(iconv_t ic, const std::string &src) {
			char *instr = (char *)src.c_str(); // ugly... but iconv requires char **
			size_t inlen = src.length();
			vararray<char> buf(src.length()*6);
			char *outstr = buf.c_array();
			size_t outlen = buf.size();
			if (iconv(ic, &instr, &inlen, &outstr, &outlen)<0) throw std::invalid_argument("cannot convert string");
			*outstr = 0;
			return buf.c_array();
		}
	}

	std::string ustring::conv_to_u8(const std::string &host_str) {
		const std::string en = get_host_enc_str();
		if (en=="UTF-8") return host_str;
		auto_iconv_t ic("UTF-8//IGNORE", en.c_str());
		return do_conv(ic, host_str);
	}

	std::string ustring::conv_from_u8(const ustring &utf8_str) {
		const std::string en = get_host_enc_str();
		if (en=="UTF-8") return utf8_str.str_;
		auto_iconv_t ic((en+"//IGNORE").c_str(), "UTF-8");
		return do_conv(ic, utf8_str.str_);
	}

	namespace fsutil {
		namespace {
			inline char ntoc(int n) { return n<10 ? n : 'z'-n; }
			inline ustring nval_to_str(int v) {
				char buf[5];
				buf[4] = 0;
				buf[3] = ntoc(v%36); v /= 36;
				buf[2] = ntoc(v%36); v /= 36;
				buf[1] = ntoc(v%36); v /= 36;
				buf[0] = ntoc(v%36); v /= 36;
				return ustring(buf, enc_utf_8);
			}
		}
		ustring get_temporary_name(const ustring &where, const ustring &prefix, bool create) {
			for (int n=0; n<26*26*26*26; ++n) {
				const ustring f = fsutil::add_trailing_slash(where) + prefix + nval_to_str(n) + ustring(".tmp", enc_utf_8);
				const int n = open(f.a_str().c_str(), O_RDWR | (create?O_CREAT:0));
				if (n>0) {
					close(n);
					return f;
				}
			}
			throw std::runtime_error("cannot create temporary file name at " + where.a_str() + '\\' + prefix.a_str() + "XXXX.TMP");
		}
		bool symlink_usable(const ustring &where) {
			const ustring trgname = get_temporary_name(where, "tht", true);
			const ustring lnkname = get_temporary_name(where, "thl");
			if (!link(trgname.a_str().c_str(), lnkname.a_str().c_str())) {
				unlink(lnkname.a_str().c_str());
				unlink(trgname.a_str().c_str());
				return true;
			}
			return false;
		}
		bool hardlink_usable(const ustring &where) {
			const ustring trgname = get_temporary_name(where, "tht", true);
			const ustring lnkname = get_temporary_name(where, "thl");
			if (!symlink(trgname.a_str().c_str(), lnkname.a_str().c_str())) {
				unlink(lnkname.a_str().c_str());
				unlink(trgname.a_str().c_str());
				return true;
			}
			return false;
		}
		bool unix_perm_usable(const ustring &) { return true; }
		bool ignore_case_path(const ustring &) { return true; }

		ustring add_trailing_slash(const ustring &dir) {
			return del_trailing_slash(dir) + ustring("/", enc_utf_8);
		}

		ustring del_trailing_slash(const ustring &dir) {
    		if (dir.empty()) return dir;
			switch (*dir.crbegin()) {
			case '/':
			case '\\':
				return del_trailing_slash(ustring(std::string(dir.u_str(), 0, dir.length()-1), enc_utf_8));
			}
			return dir;
		}
	}
}
