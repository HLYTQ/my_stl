#ifndef _STRING_H_
#define _STRING_H_

#include "basic_string.h"

namespace tstl{

using string = tstl::basic_string<char>;
using wstring = tstl::basic_string<wchar_t>;
using u16string = tstl::basic_string<char16_t>;
using u32string = tstl::basic_string<char32_t>;

// if C++ >=20 have char8_t;
// using string = tstl::char_traits<char8_t>;

} // end tstl

#endif // _STRING_H_
