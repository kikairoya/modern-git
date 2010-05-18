#include "git-compat-util.hpp"
#define BOOST_TEST_MAIN
#include "unittest.hpp"

#include <string.h>

BOOST_AUTO_TEST_CASE(ustring_test) {
	using mgit::ustring;
	{
		const ustring s("");
		BOOST_CHECK(s.empty());
		BOOST_CHECK_EQUAL(s.length(), 0);
	} {
		const char *const sc = "0";
		const ustring s(sc);
		BOOST_CHECK(!s.empty());
		BOOST_CHECK_EQUAL(s.length(), 1);
		BOOST_CHECK(!strcmp(s.a_str().c_str(), sc));
	} {
		const char *const aiueo_sj = "\x82\xa0\x82\xa2\x82\xa4\x82\xa6\x82\xa8";
		const char *const aiueo_ej = "\xa4\xa2\xa4\xa4\xa4\xa6\xa4\xa8\xa4\xaa";
		const char *const aiueo_u8 = "\xe3\x81\x82\xe3\x81\x84\xe3\x81\x86\xe3\x81\x88\xe3\x81\x8a";
#if defined(__MINGW32__) || defined(_MSC_VER) || defined(__CYGWIN__)
#define STR aiueo_sj
#elif EUCJP
#define STR aiueo_ej
#else
#define STR aiueo_u8
#endif
		const ustring s1(STR);
		BOOST_CHECK(!s1.empty());
		BOOST_CHECK_EQUAL(s1.length(), 15);
		BOOST_CHECK(!strcmp(s1.a_str().c_str(), STR));
		BOOST_CHECK(!strcmp(s1.u_str(), aiueo_u8));
		const ustring s2(aiueo_u8, 0);
		BOOST_CHECK(!s2.empty());
		BOOST_CHECK_EQUAL(s2.length(), 15);
		BOOST_CHECK(!strcmp(s2.a_str().c_str(), STR));
		BOOST_CHECK(!strcmp(s2.u_str(), aiueo_u8));
		const ustring s3(s1 + s2);
		const std::string ss(std::string(STR)+STR);
		BOOST_CHECK_EQUAL(s3.length(), 30);
		BOOST_CHECK(!strcmp(s3.a_str().c_str(), ss.c_str()));
		BOOST_CHECK(!strcmp(s3.u_str(), std::string(aiueo_u8).append(aiueo_u8).c_str()));
		BOOST_CHECK(s1 == s2);
		BOOST_CHECK(!(s1 != s2));
		BOOST_CHECK(!(s1 == s3));
		BOOST_CHECK(s1 != s3);
	}
}
