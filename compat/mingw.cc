#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#define _WIN32_IE 0x0600

#include "git-compat-util.hpp"
#include "path.hpp"

#include <stdexcept>
#include <windows.h>
#include <shlobj.h>

#ifdef _MSC_VER
#pragma comment(lib,"shell32.lib")
#endif

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

		ustring get_exec_path() {
			DWORD bufsiz = MAX_PATH;
			while (1) {
				vararray<char> buf(bufsiz);
				DWORD len = GetModuleFileNameA(NULL, buf.c_array(), buf.size());
				if (!len) throw std::runtime_error("couldn't get exec-path");
				if (len < buf.size()) return buf.c_array();
				bufsiz *= 2;
			}
		}

		ustring get_user_config_dir() {
			WCHAR dir[MAX_PATH];
			if (!SHGetSpecialFolderPathW(NULL, dir, CSIDL_LOCAL_APPDATA, TRUE)) throw std::runtime_error("cannot get CSIDL_LOCAL_APPDATA");
			return add_trailing_slash(ustring(conv_from_utf16(dir, enc_utf_8), enc_utf_8)) + "/modern-git/config";
		}

	}
}
