// contain the basic_string and char_traits

/**
 * The noexcept declare seems like can make function 
 * more fast, I don't know exactly how to use it
*/

#ifndef BASIC_STRING_H_
#define BASIC_STRING_H_

#include "iterator"
#include "allocator.h"
#include "iterator.h"
#include "algorithm.h"
#include <cassert>
#include <iostream>
#include <cstring>

namespace tstl{

  // every char_type has different length
  // so, is't very pain!!!
  // you have to coding for every situation

  // There some type traits for most of char type

  template<typename _ch>
  struct char_traits{
    using char_type = _ch;
    using char_pointer = _ch*;

    static size_t len(const char_pointer str){
      size_t len = 0;
      for(;*str != char_type(0); ++str)
        return ++len; 
      return len;
    }

    static 
    int compare(const char_pointer rhs, const char_pointer lhs, size_t n)
    {
    
      for(;n != 0; --n, rhs++,lhs++){
        if(*rhs < *lhs) return -1;
        if(*rhs > *lhs) return 1;
      }
      return 0;
    } 

    // this function without boundary check, becareful!!!
    static 
    char_pointer copy(char_pointer dst, const char_pointer src, size_t n)
    {
      char_pointer tmp = dst;
      for(;n != 0; --n, ++dst, ++src){
        *dst = * src;
      }
      return tmp;
    }

    static 
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

    static 
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

    static size_t len(const char_type* str)
    { return std::strlen(str); }

    static 
    int compare(const char_type* lhs, const char_type* rhs)
    { return std::strcmp(lhs, rhs); }

    static 
    char_pointer copy(char_pointer dst, const char_type* src, size_t n)
    { return static_cast<char_pointer>(std::memcpy(dst,src,n)); }

    static 
    char_pointer move(char_pointer dst, const char_pointer src, size_t n)
    { return static_cast<char_pointer>(std::memmove(dst,src,n)); }

    static 
    char_pointer fill(char_pointer dst, const char_type ch, size_t n)
    { return static_cast<char_pointer>(std::memset(dst,ch,n)); }

  };

  template<>
  struct char_traits<wchar_t>{
    using char_type = wchar_t;
    using char_pointer = char_type*;

    static size_t len(const char_type* str)
    { return std::wcslen(str); }

    static int compare(const char_type* lhs, const char_type* rhs, size_t n)
    { return std::wmemcmp(lhs,rhs,n);}

    static char_pointer copy(char_type* dst, const char_type* src, size_t n)
    { return static_cast<char_pointer>(std::wmemcpy(dst,src,n)); }

    static char_pointer move(char_type* dst, const char_type* src, size_t n)
    { return static_cast<char_pointer>(std::wmemmove(dst,src,n)); }

    static char_pointer fill(char_type* dst, const char_type ch, size_t n)
    { return static_cast<char_pointer>(std::wmemset(dst,ch,n)); }
  };

  // C++20 has char8_t, but I think I'll need it.
  // template<>
  // struct char_traits<char8_t>{
  //   using char_type = char8_t;
  // };

  template<>
  struct char_traits<char16_t>{
    using char_type = char16_t;
    using char_pointer = char_type*;

     static 
    size_t len(const char_type* str) noexcept {
      size_t len = 0;
      for (; *str != char_type(0); ++str)
        ++len;
      return len;
    }

     static 
    int compare(const char_type* s1, const char_type* s2, size_t n) noexcept {
      for (; n != 0; --n, ++s1, ++s2){
        if (*s1 < *s2) return -1;
        if (*s2 < *s1) return 1;
      }
      return 0;
    }

     static 
    char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept {
      char_type* r = dst;
      for (; n != 0; --n, ++dst, ++src)
        *dst = *src;
      return r;
    }

     static 
    char_type* move(char_type* dst, const char_type* src, size_t n) noexcept {
      char_type* r = dst;
      if (dst < src) {
        for (; n != 0; --n, ++dst, ++src)
          *dst = *src;
      }
      else if (src < dst) {
        dst += n;
        src += n;
        for (; n != 0; --n)
          *--dst = *--src;
      }
      return r;
    }

     static 
    char_type* fill(char_type* dst, char_type ch, size_t count) noexcept {
      char_type* r = dst;
      for (; count > 0; --count, ++dst)
        *dst = ch;
      return r;
    }
  };

  template<>
  struct char_traits<char32_t>{
    using char_type = char32_t;
    using char_pointer = char_type*;

     static 
    size_t len(const char_type* str) noexcept {
      size_t len = 0;
      for (; *str != char_type(0); ++str)
        ++len;
      return len;
    }

     static 
    int compare(const char_type* s1, const char_type* s2, size_t n) noexcept {
      for (; n != 0; --n, ++s1, ++s2){
        if (*s1 < *s2) return -1;
        if (*s2 < *s1) return 1;
      }
      return 0;
    }

     static 
    char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept {
      char_type* r = dst;
      for (; n != 0; --n, ++dst, ++src)
        *dst = *src;
      return r;
    }

     static 
    char_type* move(char_type* dst, const char_type* src, size_t n) noexcept {
      char_type* r = dst;
      if (dst < src) {
        for (; n != 0; --n, ++dst, ++src)
          *dst = *src;
      }
      else if (src < dst) {
        dst += n;
        src += n;
        for (; n != 0; --n)
          *--dst = *--src;
      }
      return r;
    }

     static 
    char_type* fill(char_type* dst, char_type ch, size_t count) noexcept {
      char_type* r = dst;
      for (; count > 0; --count, ++dst)
        *dst = ch;
      return r;
    }
  };
 
template <typename _Char, 
          typename _Traits = char_traits<_Char>, 
          typename _Alloc = tstl::allocator<_Char>>
class basic_string{
public:
  using data_allocator = _Alloc;

  using traits_type     = _Traits;
  using char_type       = typename data_allocator::value_type;
  using size_type       = typename data_allocator::difference_type; 
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
   basic_string() { }

  /**
   * because to avoid pos large than string's size
   * I change size_type from size_t to difference_t
   * so m_size's value at least zero
   */

  basic_string(const basic_string& other, size_type pos){
    m_size = other.m_size - pos;
    choose_init_on_hs(other.m_str + pos, tstl::max(m_size,size_type(0)));
  }

  basic_string(const basic_string& other, size_type pos, size_type count){
      if(other.m_size - (pos + count) >= 0)
        m_size = count;
      else m_size = 0;
      init_on_heap(other.m_str + pos,m_size);
  }

  basic_string(const_pointer str){
    init_from(str);
  }

  basic_string(const_pointer str, size_type count){
    m_size = char_traits<char_type>::len(str) - count;
    choose_init_on_hs(str + count,tstl::max(m_size,size_type(0)));
  }

  template<class Iter, typename std::enable_if<
        tstl::is_input_iterator<Iter>::value, int>::type = false>
  basic_string(Iter first, Iter second){
    m_size = second - first;
    init_on_heap(first, tstl::max(m_size,size_type(0)));
  }
 
  basic_string(const basic_string& other);

  basic_string(basic_string&& other);

  basic_string& operator=(const basic_string& other);
  
  basic_string& operator=(basic_string&& other);
  
  // if use C++20, I can have  destruct function
  ~basic_string() noexcept
  { destory_string(); }

  // iterator 
  iterator begin() noexcept
  { return iterator(m_str); }

  const_iterator begin() const noexcept
  { return const_iterator(m_str); }

  iterator end() noexcept
  { return iterator(m_str + m_size); }

  const_iterator end() const noexcept
  { return const_iterator(m_str + m_size); }

  reverse_iterator rbegin() noexcept
  { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const noexcept
  { return const_reverse_iterator(end()); }

  reverse_iterator rend() noexcept
  { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const noexcept
  { return const_reverse_iterator(begin()); }
  
  const_iterator operator-(const difference_type off) const {  
    const_iterator tmp = m_str;
    tmp -= off;
    return tmp;
  }

  const_iterator operator+(const difference_type off) const {  
    const_iterator tmp = m_str;
    tmp += off;
    return tmp;
  }

  // some basic function
  
  size_t size() 
  { return static_cast<size_t>(m_size); }

  // helper function
  // BECAREFUL!!! every operate on string must through char_traits

private:
  // I use to desgined init_on_stack when string length < 15
  // BUT I FAIL!!! I don't know why

   void init_on_heap(const_pointer str, size_type count) {
    m_capacity = count;
    pointer tmp = data_allocator::allocate(m_capacity);
    traits_type::copy(tmp,str,count);
    m_str = tmp;
  }

   void init_from(const_pointer str) {
    m_size = traits_type::len(str);
    m_capacity = m_size;
    init_on_heap(str,m_size);
  }

   void destory_string(){
    if(m_size <= 15)
      return;
    data_allocator::deallocate(m_str);
    m_size = m_capacity = 0;
  }

   void realloc(const size_type newCapacity){
    pointer newBlock = data_allocator::allocate(newCapacity);
        
    if(newCapacity < m_size) 
        m_size = newCapacity;
        
    for(size_t i =  0; i < m_size; ++i)
        tstl::construct(newBlock + i, m_str[i]);
        
    tstl::destroy(m_str, m_str + m_size);

    m_str = newBlock;
    m_capacity = newCapacity;
  }

public: 
  // read function
   reference operator[](const size_type index){
    assert(index < m_size);
    return m_str[index];
  }

   const_reference operator[](const size_type index) const {
    assert(index < m_size);
    return m_str[index];
  }
  template <typename ch>
  friend std::ostream& operator<<(std::ostream& stream, const basic_string<ch>& string);

};
  template <typename ch>
  std::ostream& operator<<(std::ostream& stream, const basic_string<ch>& string){
    stream<<string.m_str;
    return stream;
  }
} // end namespace tstl

#endif // BASIC_STRING_H
