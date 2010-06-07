#include "config.hpp"
#define BOOST_TEST_MODULE config
#include "unittest.hpp"

BOOST_AUTO_TEST_CASE(config_test) {
	{
		char* ph = boost::unit_test::framework::master_test_suite().argv[1];
		mgit::git_config_map m = mgit::read_config_file(ph);

		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("", "", "")],
			mgit::git_config_value()
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("core", "", "str")],
			mgit::git_config_value(mgit::ustring("value"))
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("core", "", "num")],
			mgit::git_config_value(10LL)
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("sec2", "subsec1", "subname1")],
			mgit::git_config_value(mgit::ustring("subvalue1"))
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("cc", "", "cc")],
			mgit::git_config_value(mgit::ustring("cc"))
		);
	}
}
