// string header file
// contain the basic_string and char_traits

#ifndef _STRING_H_
#define _STRING_H_

#include "iterator"
#include "allocator.h"
#include "iterator.h"
#include <cstring>

namespace tstl{

  // every char_type has different length
  // so, to coding char_traits is very pain!!!

  template<typename _ch>
  struct char_traits{
    using char_type = _ch;
    using char_pointer = _ch*;

    constexpr static size_t len(const char_pointer str){
      size_t len = 0;
      for(;*str != char_type(0); ++str)
        return ++len; 
      return len;
    }

    constexpr static 
    int compare(const char_pointer rhs, const char_pointer lhs, size_t n)
    {
    
      for(;n != 0; --n, rhs++,lhs++){
        if(*rhs < *lhs) return -1;
        if(*rhs > *lhs) return 1;
      }
      return 0;
    } 

    // this function without boundary check, becareful!!!
    constexpr static 
    char_pointer copy(char_pointer dst, const char_pointer src, size_t n)
    {
      char_pointer tmp = dst;
      for(;n != 0; --n, ++dst, ++src){
        *dst = * src;
      }
      return tmp;
    }
    constexpr static 
    char_pointer move(char_pointer dst, const char_pointer src, size_t n)
    {
      if(n == 0) return dst;

      char_pointer r = dst;
      size_t dst_len = char_traits<_ch>::len(dst);
      size_t src_len = char_traits<_ch>::len(src);
      if (dst_len < src_len){
          for (; n != 0; --n, ++dst, ++src)
          *dst = *src;
      }
      else if (src_len < dst_len){
        dst += n;
        src += n;
        for (; n != 0; --n)
          *--dst = *--src;
      }
      return r; 
    }

    constexpr static 
    char_pointer fill(char_pointer dst,const char_pointer ch, size_t count){
      char_type* r = dst;
        for (; count > 0; --count, ++dst)
          *dst = ch;
      return r;
    }
  };

  template<>
  struct char_traits<char>{
    using char_type = char;
    using char_pointer = char_type*;

    constexpr static size_t len(const char_type* str)
    { return std::strlen(str); }

    constexpr static 
    int compare(const char_type* lhs, const char_type* rhs, size_t n)
    { return std::strcmp(lhs, rhs, n); }

    constexpr static 
    char_pointer copy(char_pointer dst, const char_pointer src, size_t n)
    { return static_cast<char_pointer>(std::memcpy(dst,src,n)); }

    constexpr static 
    char_pointer move(char_pointer dst, const char_pointer src, size_t n)
    { return static_cast<char_pointer>(std::memmove(dst,src,n)); }

    constexpr static 
    char_pointer fill(char_pointer dst, const char_pointer ch, size_t n)
    { return static_cast<char_pointer>(std::memset(dst,ch,n)); }

  };

  template<>
  struct char_traits<wchar_t>{
    using char_type = wchar_t;
    using char_pointer = char_type*;

    constexpr static size_t len(const char_type* str)
    { return static_cast<char_pointer>(std::wcslen(str)); }

    constexpr static 
    int compare(const char_type* lhs, const char_type* rhs, size_t n)
    { return std::wmemcmp(lhs,rhs,n);}

    constexpr static 
    char_pointer copy(const char_type* dst, const char_type* src, size_t n)
    { return static_cast<char_pointer>(std::memcpy(dst,src,n)); }

    constexpr static 
    char_pointer move(const char_type* dst, const char_type* src, size_t n)
    { return static_cast<char_pointer>(std::wmemmove(dst,src,n)); }

    constexpr static 
    char_pointer fill(const char_type* dst, const char_type ch, size_t n)
    { return static_cast<char_pointer>(std::wmemset(dst,ch,n)); }
  };

  // if C++ 20: has char8_t
  // template<>
  // struct char_traits<char8_t>{
  //   using char_type = char8_t;
  // };

  template<>
  struct char_traits<char16_t>{
    using char_type = char16_t;
    using char_pointer = char_type*;
    constexpr static size_t len();
    constexpr static int compare();
    constexpr static char_pointer copy();
    constexpr static char_pointer move();
    constexpr static char_pointer fill();
  };

  template<>
  struct char_traits<char32_t>{
    using char_type = char32_t;
    using char_pointer = char_type*;

    constexpr static size_t len();
    constexpr static int compare();
    constexpr static char_pointer copy();
    constexpr static char_pointer move();
    constexpr static char_pointer fill();
  };
 
template<typename _Char, 
          typename _Traits = char_traits<_Char>, 
          typename _Alloc = tstl::allocator<_Char>>
class basic_string{
public:
  using data_allocator = _Alloc;

  using traits_type     = _Traits;
  using char_type      = typename data_allocator::value_type;
  using size_type       = typename data_allocator::size_type;
  using difference_type = typename data_allocator::difference_type;
  using reference       = typename data_allocator::reference;
  using const_reference = typename data_allocator::const_reference;
  using pointer         = typename data_allocator::pointer;
  using const_pointer   = typename data_allocator::const_pointer;
  
  using iterator               = pointer;
  using const_iterator         = const_pointer;
  using reverse_iterator       = tstl::reverse_iterator<iterator>;
  using const_reverse_iterator = tstl::reverse_iterator<const_iterator>;
 
private:

  pointer m_str = nullptr;
  size_type m_size = 0;
  size_type m_capacity = 0;

public:
  // construct function:
  constexpr basic_string(){
    try_init();
  }

  constexpr basic_string(const basic_string& other, size_type pos);

  constexpr basic_string(const basic_string& other, size_type pos, size_type count);

  constexpr basic_string(const_pointer str){
    init_from();
  }
  constexpr basic_string(const_pointer str, size_type count);

  template<class Iter, typename std::enable_if<
        tstl::is_input_iterator<Iter>::value, int>::type = false>
  constexpr basic_string(Iter first, Iter second);
  
  constexpr basic_string(const basic_string& other);

  constexpr basic_string(basic_string&& other);

  constexpr basic_string& operator=(const basic_string& other);
  
  constexpr basic_string& operator=(basic_string&& other);

  // helper function
  constexpr void init_from();

  constexpr void try_init();

  constexpr void fill_init();

  // read function
  constexpr reference operator[](const size_type index){
    assert(index < m_size);
    return m_str[index];
  }

  constexpr const_reference operator[](const size_type index) const {
    return m_str[index];
  }
};  
  
} // end namespace tstl

#endif // _STRING_H