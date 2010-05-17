#ifdef HAS_BUILT_BOOST_LIBS

#include <boost/test/unit_test.hpp>

#else

#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>

#endif
