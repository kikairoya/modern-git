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
		inline std::string get_nl_langinfo_codeset() {
#ifdef OLD_CYGWIN
			return "CP932"; // FIXME!!!
#else
			setlocale(LC_ALL, "");
			const char *s = nl_langinfo(CODESET);
			return s ? s : "UTF-8";
#endif
		}
		std::string get_host_enc_str() {
			static const std::string code_str = get_nl_langinfo_codeset();
			if (code_str=="CP720") return "WINDOWS-1256";
			return code_str;
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
			inline char ntoc(unsigned n) { return n<10 ? n+'a' : 'z'-n; }
			inline ustring nval_to_str(unsigned v) {
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
			for (unsigned n=0; n<26*26*26*26; ++n) {
				const ustring f = fsutil::add_trailing_slash(where) + prefix + nval_to_str(n) + ustring(".tmp", enc_utf_8);
				const int fd = open(f.a_str().c_str(), O_RDWR | (create?O_CREAT:0));
				if (fd>0) {
					close(fd);
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
		bool unix_perm_usable(const ustring &) { return false; }
		bool ignore_case_path(const ustring &) { return false; }

	}
}
