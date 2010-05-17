#ifndef GIT_COMPAT_UTIL_HPP_
#define GIT_COMPAT_UTIL_HPP_

#include <string>
#include <iterator>
#include <memory>
#include <vector>
#include <list>

#ifdef HAS_CXX0X_MEMORY
namespace mgit {
# ifdef HAS_CXX0X_TR1
	using std::tr1::shared_ptr;
	using std::tr1::make_shared;
# else
	using std::shared_ptr;
	using std::make_shared;
# endif
}
#else
# ifdef HAS_BOOST
# include <boost/shared_ptr.hpp>
namespace mgit {
	using boost::shared_ptr;
	using boost::make_shared;
}
# else
# error we need the 'shared_ptr' implementation.
# endif
#endif

namespace mgit {
	using std::list;

	/** @brief a main string type of internal use
	 *
	 * ustring's encoding is always utf-8. std::string's is host's locale.
	 **/
	class ustring {
	public:
		ustring(): str_() { }
		/*explicit*/ ustring(const std::string &host_str): str_(conv_to_u8(host_str)) { }
		/*explicit*/ operator std::string() const { return conv_from_u8(*this); }
		const char *u_str() const { return str_.c_str(); }
		size_t length() const { return str_.length(); }
	private:
		static std::string conv_to_u8(const std::string &host_str);
		static std::string conv_from_u8(const ustring &utf8_str);
	private:
		std::string str_;
	};

	template <typename T>
	class vararray {
	public:
		typedef T value_type;
		typedef T element_type;
		typedef T *pointer;
		typedef const T *const_pointer;
		typedef T &reference;
		typedef const T &const_reference;
		typedef pointer iterator;
		typedef const_pointer const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	public:
		explicit vararray(size_t n): size_(n), elem_(new T[n]) { }
		~vararray() { delete[] elem_; }
		const size_t size() const { return size_; }
		pointer c_array() const { return elem_; }
		reference operator [](size_t n) { return elem_[n]; }
		const_reference operator [](size_t n) const { return elem_[n]; }
		iterator begin() { return elem_; }
		const_iterator begin() const { return elem_; }
		const_iterator cbegin() const { return elem_; }
		iterator end() { return elem_+size_; }
		const_iterator end() const { return elem_+size_; }
		const_iterator cend() const { return elem_+size_; }
	private:
		vararray(const vararray &);
		vararray &operator =(const vararray &);
	private:
		size_t size_;
		T *elem_;
	};

}

#endif
