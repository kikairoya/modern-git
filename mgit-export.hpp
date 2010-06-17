#ifndef MGIT_EXPORT_HPP_
#define MGIT_EXPORT_HPP_

#include <boost/config.hpp>

#if defined(BOOST_HAS_DECLSPEC)
    #if defined(MGIT_DYN_LINK)
        #if defined(MGIT_SOURCE)
            #define MGIT_DECL __declspec(dllexport)
        #else
            #define MGIT_DECL __declspec(dllimport)
        #endif
    #endif
#endif // defined(BOOST_HAS_DECLSPEC)

#if !defined(MGIT_DECL)
    #define MGIT_DECL
#endif

#endif // MGIT_EXPORT_HPP_
