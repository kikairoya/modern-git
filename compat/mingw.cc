#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>
#include <windows.h>

namespace mgit {

	namespace {
		std::wstring conv_to_utf16(const std::string &mbstr, DWORD codepage) {
			const int r = MultiByteToWideChar(codepage, 0, mbstr.c_str(), mbstr.length(), NULL, 0);
			if (r<0) throw std::invalid_argument("cannot convert from " + lexical_cast<std::string>(codepage) + " to UTF-16");
			vararray<wchar_t> buf(r+2);
			MultiByteToWideChar(codepage, 0, mbstr.c_str(), mbstr.length(), buf.c_array(), buf.size());
			buf[r] = 0;
			return std::wstring(buf.c_array());
		}
		std::string conv_from_utf16(const std::wstring &wstr, DWORD codepage) {
			const int r = WideCharToMultiByte(codepage, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
			if (r<0) throw std::invalid_argument("cannot convert to" + lexical_cast<std::string>(codepage) + "from UTF-16");
			vararray<char> buf(r+2);
			WideCharToMultiByte(codepage, 0, wstr.c_str(), wstr.length(), buf.c_array(), buf.size(), NULL, NULL);
			buf[r] = 0;
			return std::string(buf.c_array());
		}
	}
	
	std::string ustring::conv_to_u8(const std::string &host_str) {
		return conv_from_utf16(conv_to_utf16(host_str, CP_ACP), CP_UTF8);
	}

	std::string ustring::conv_from_u8(const ustring &utf8_str) {
		return conv_from_utf16(conv_to_utf16(utf8_str.str_, CP_UTF8), CP_ACP);
	}

	std::wstring ustring::conv_to_u16(const std::string &host_str) {
		return conv_to_utf16(host_str, CP_ACP);
	}

	std::wstring ustring::conv_to_u16(const ustring &utf8_str) {
		return conv_to_utf16(utf8_str.str_, CP_UTF8);
	}

	namespace fsutil {
		ustring get_temporary_name(const ustring &where, const ustring &prefix, bool create) {
			char buf[MAX_PATH];
			if (GetTempFileNameA(where.a_str().c_str(), prefix.a_str().c_str(), 0, buf)) {
				if (!create) DeleteFileA(buf);
				return ustring(buf);
			} else {
				throw std::runtime_error("cannot create temporary file name at " + where.a_str() + '\\' + prefix.a_str() + "XXXX.TMP");
			}
		}
		bool symlink_usable(const ustring &where) {
			const ustring trgname = get_temporary_name(where, "tht", true);
			const ustring lnkname = get_temporary_name(where, "thl");
			HMODULE h = GetModuleHandle("kernel32.dll");
			typedef BOOL (*T)(LPCWSTR, LPCWSTR, LPVOID);
			T pCreateSymbolicLinkW = (T)GetProcAddress(h, "CreateSymbolicLinkW");
			if (pCreateSymbolicLinkW && pCreateSymbolicLinkW(lnkname.w_str().c_str(), trgname.w_str().c_str(), NULL)) {
				DeleteFileW(lnkname.w_str().c_str());
				DeleteFileW(trgname.w_str().c_str());
				return true;
			}
			return false;
		}
		bool hardlink_usable(const ustring &where) {
			const ustring trgname = get_temporary_name(where, "tht", true);
			const ustring lnkname = get_temporary_name(where, "thl");
			HMODULE h = GetModuleHandleA("kernel32.dll");
			typedef BOOL (*T)(LPCWSTR, LPCWSTR, LPVOID);
			T pCreateHardLinkW = (T)GetProcAddress(h, "CreateHardLinkW");
			if (pCreateHardLinkW && pCreateHardLinkW(lnkname.w_str().c_str(), trgname.w_str().c_str(), NULL)) {
				DeleteFileW(lnkname.w_str().c_str());
				DeleteFileW(trgname.w_str().c_str());
				return true;
			}
			return false;
		}
		bool unix_perm_usable(const ustring &) { return false; }
		bool ignore_case_path(const ustring &) { return false; }

		ustring add_trailing_slash(const ustring &dir) {
			return del_trailing_slash(dir) + ustring("\\", enc_utf_8);
		}

		ustring del_trailing_slash(const ustring &dir) {
			switch (*dir.crbegin()) {
			case '/':
			case '\\':
				return del_trailing_slash(ustring(std::string(dir.u_str(), 0, dir.length()-1), enc_utf_8));
			}
			return dir;
		}
	}
}
