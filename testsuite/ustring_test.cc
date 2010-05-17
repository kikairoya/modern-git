#include "git-compat-util.hpp"
#define BOOST_TEST_MAIN
#include "unittest.hpp"

#include <string.h>

BOOST_AUTO_TEST_CASE(ustring_test) {
	using mgit::ustring;
	{
		ustring s;
		BOOST_CHECK(s.empty());
		BOOST_CHECK_EQUAL(s.length(), 0);
	} {
		const char *const sc = "0";
		ustring s(sc);
		BOOST_CHECK(!s.empty());
		BOOST_CHECK_EQUAL(s.length(), 1);
		BOOST_CHECK(!strcmp(s.a_str().c_str(), sc));
	}
}
