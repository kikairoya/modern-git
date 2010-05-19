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
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc").u_str(), "abc"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc/").u_str(), "abc"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc\\").u_str(), "abc"));
		BOOST_CHECK(!strcmp(mgit::fsutil::del_trailing_slash("abc/\\").u_str(), "abc"));
	}
}
