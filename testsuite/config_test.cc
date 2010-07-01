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
			m[mgit::git_config_name("core", "", "ignorecase")],
			mgit::git_config_value(true)
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("core", "", "repositoryFormatVersion")],
			mgit::git_config_value(0.0)
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("core", "", "editor")],
			mgit::git_config_value(mgit::ustring("vi"))
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("other", "subsec", "cfg")],
			mgit::git_config_value(mgit::ustring("value"))
		);
		BOOST_CHECK_EQUAL(
			m[mgit::git_config_name("other2", "subsec", "cfg")],
			mgit::git_config_value(mgit::ustring("value"))
		);
	}
}
