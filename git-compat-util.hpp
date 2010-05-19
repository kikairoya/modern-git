#ifndef GIT_COMPAT_UTIL_HPP_
#define GIT_COMPAT_UTIL_HPP_

#include <string>
#include <iterator>
#include <map>
#include <memory>
#include <vector>
#include <list>
#include <sstream>
#include <boost/lexical_cast.hpp>

#ifdef HAS_CXX0X_MEMORY
namespace mgit {
# ifdef HAS_CXX0X_TR1
	using std::tr1::shared_ptr;
# else
	using std::shared_ptr;
# endif
}
#else
# include <boost/shared_ptr.hpp>
namespace mgit {
	using boost::shared_ptr;
}
#endif

namespace mgit {
	using std::map;
	using std::list;
	using std::vector;
	using std::stringstream;
	using boost::lexical_cast;

	enum encoding {
		enc_host_native,
		enc_utf_8,
	};

	/** @brief a main string type of internal use
	 *
	 * Ustring's encoding is always utf-8. std::string's is host's locale.
	 * To avoid implicitly confusing encode, I think this class would be immutable.
	 **/
	class ustring {
	public:
		/// Default ctor with empty string. std::map requires this.
		ustring(): str_() { }

		/// Construct from host's encoding string. Is this need explicit ?
		ustring(const std::string &str, encoding src_enc = enc_host_native)
			 : str_(src_enc==enc_host_native ? conv_to_u8(str) : str) { }

		/// same for usablility.
		ustring(const char *str, encoding src_enc = enc_host_native)
			 : str_(src_enc==enc_host_native ? conv_to_u8(str) : str) { }

		/// Copy constructor
		ustring(const ustring &other): str_(other.str_) { }

		/// Copy assignment operator
		ustring &operator =(const ustring &other) { str_ = other.str_; return *this; }

		/// Converting assignment operator
		ustring &operator =(const std::string &host_str) { str_ = conv_to_u8(host_str); return *this; }

		/// Converting assignment operator
		ustring &operator =(const char *host_str) { str_ = host_str ? conv_to_u8(host_str) : ""; return *this; }

		// Implicit conversion to host's encoding, but I think this should be disabled.
		// operator std::string() const { return conv_from_u8(*this); }

		/// Get a string by host's encoding.
		std::string a_str() const { return conv_from_u8(*this); }

		/// Get a wstring by host's wide-encoding.
		std::wstring w_str() const { return conv_to_u16(*this); }

		/// Get a byte-stream with utf-8 encoding.
		const char *u_str() const { return str_.c_str(); }

		/// Get length in utf-8 encoding.
		size_t length() const { return str_.length(); }

		/// Tell string is empty.
		bool empty() const { return str_.empty(); }

		/// Subscript operator
		char operator [](size_t n) const { return str_[n]; }

		/// Concat two utf-8 strings.
		friend const ustring operator +(const ustring &x, const ustring &y) { return ustring(x.str_ + y.str_, enc_utf_8); }

		/// Compare two utf-8 strings.
		friend bool operator ==(const ustring &x, const ustring &y) { return x.str_ == y.str_; }
		friend bool operator !=(const ustring &x, const ustring &y) { return x.str_ != y.str_; }

		/// Stream output with utf-8 encoding. I don't know we need this operator...
		friend std::ostream &operator <<(std::ostream &os, const ustring &s) { return os << s.str_; }

		/// swap
		void swap(ustring &other) { std::swap(str_, other.str_); }
	private:
		static std::string conv_to_u8(const std::string &host_str);
		static std::string conv_from_u8(const ustring &utf8_str);
		static std::wstring conv_to_u16(const std::string &host_str);
		static std::wstring conv_to_u16(const ustring &utf8_str);
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


	bool filemode_trustable();
}

namespace std {
	template <>
	inline void swap<mgit::ustring>(mgit::ustring &x, mgit::ustring &y) { x.swap(y); }
}

#endif
