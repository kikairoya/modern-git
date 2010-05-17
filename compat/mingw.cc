#include <windows.h>
#include "git-compat-util.hpp"

#include <stdexcept>

namespace mgit {

	namespace {
		std::wstring conv_to_utf16(const std::string &mbstr, DWORD codepage) {
			if (const int r = MultiByteToWideChar(codepage, 0, mbstr.c_str(), mbstr.length(), NULL, 0)) {
				vararray<wchar_t> buf(r+2);
				MultiByteToWideChar(codepage, 0, mbstr.c_str(), mbstr.length(), buf.c_array(), buf.size());
				buf[r] = 0;
				return std::wstring(buf.c_array());
			} else {
				throw std::invalid_argument("cannot convert from" + lexical_cast<std::string>(codepage) + "to UTF-16");
			}
		}
		std::string conv_from_utf16(const std::wstring &wstr, DWORD codepage) {
			if (const int r = WideCharToMultiByte(codepage, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL)) {
				vararray<char> buf(r+2);
				WideCharToMultiByte(codepage, 0, wstr.c_str(), wstr.length(), buf.c_array(), buf.size(), NULL, NULL);
				buf[r] = 0;
				return std::string(buf.c_array());
			} else {
				throw std::invalid_argument("cannot convert to" + lexical_cast<std::string>(codepage) + "from UTF-16");
			}
		}
	}
	
	std::string ustring::conv_to_u8(const std::string &host_str) {
		return conv_from_utf16(conv_to_utf16(host_str, CP_ACP), CP_UTF8);
	}

	std::string ustring::conv_from_u8(const ustring &utf8_str) {
		return conv_from_utf16(conv_to_utf16(utf8_str.str_, CP_UTF8), CP_ACP);
	}

}
