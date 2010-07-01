#include "git-compat-util.hpp"
#include "path.hpp"
#define BOOST_TEST_MODULE fsutil
#include "unittest.hpp"

BOOST_AUTO_TEST_CASE(fsutil_test) {
	{
#if defined(__MINGW32__) || defined(_MSC_VER)
#define SLASH "\\"
#else
#define SLASH "/"
#endif
		BOOST_CHECK(!strcmp(mgit::fsutil::add_trailing_slash("abc").u_str(), "abc" SLASH));
		BOOST_CHECK(!strcmp(mgit::fsutil::add_trailing_slash("abc/").u_str(), "abc" SLASH));
		BOOST_CHECK(!strcmp(mgit::fsutil::add_trailing_slash("abc\\").u_str(), "abc" SLASH));
		BOOST_CHECK(!strcmp(mgit::fsutil::add_trailing_slash("abc/\\").u_str(), "abc" SLASH));
		BOOST_CHECK(!strcmp(mgit::fsutil::add_trailing_slash("/").u_str(), "/"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("").u_str(), ""));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc").u_str(), "abc"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc/").u_str(), "abc"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc\\").u_str(), "abc"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc/\\").u_str(), "abc"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("/").u_str(), "/"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("").u_str(), ""));
		BOOST_CHECK(!strcmp(mgit::fsutil::remove_file_name("/usr/lib").u_str(), "/usr"));
		BOOST_CHECK(!strcmp(mgit::fsutil::remove_file_name("/usr/").u_str(), SLASH));
		BOOST_CHECK(!strcmp(mgit::fsutil::remove_file_name("usr").u_str(), "."));
		BOOST_CHECK(!strcmp(mgit::fsutil::remove_file_name("/").u_str(), SLASH));
		BOOST_CHECK(!strcmp(mgit::fsutil::remove_file_name(".").u_str(), "."));
		BOOST_CHECK(!strcmp(mgit::fsutil::remove_file_name("..").u_str(), "."));
		BOOST_CHECK(!strcmp(mgit::fsutil::extract_file_name("/usr/lib").u_str(), "lib"));
		BOOST_CHECK(!strcmp(mgit::fsutil::extract_file_name("/usr/").u_str(), "usr"));
		BOOST_CHECK(!strcmp(mgit::fsutil::extract_file_name("usr").u_str(), "usr"));
		BOOST_CHECK(!strcmp(mgit::fsutil::extract_file_name("/").u_str(), SLASH));
		BOOST_CHECK(!strcmp(mgit::fsutil::extract_file_name(".").u_str(), "."));
		BOOST_CHECK(!strcmp(mgit::fsutil::extract_file_name("..").u_str(), ".."));
	}
}
