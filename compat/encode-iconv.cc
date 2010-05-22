#include "git-compat-util.hpp"

#include <stdexcept>

#include <langinfo.h>
#include <iconv.h>
#include <locale.h>

namespace mgit {
	namespace {
		// copy from Hamigaki.Charset
		inline std::size_t indirect_iconv(
			size_t (*func)(iconv_t, char**, size_t*, char**, size_t*),
			iconv_t cd,
			char** inbuf, std::size_t* inbytesleft,
			char** outbuf, std::size_t* outbytesleft)
		{
			return (*func)(cd, inbuf, inbytesleft, outbuf, outbytesleft);
		}

		inline std::size_t indirect_iconv(
			size_t (*func)(iconv_t, const char**, size_t*, char**, size_t*),
			iconv_t cd,
			char** inbuf, std::size_t* inbytesleft,
			char** outbuf, std::size_t* outbytesleft)
		{
			return (*func)(cd,
				const_cast<const char**>(inbuf), inbytesleft, outbuf, outbytesleft);
		}
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
			if (indirect_iconv(iconv, ic, &instr, &inlen, &outstr, &outlen)<0) throw std::invalid_argument("cannot convert string");
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

}
