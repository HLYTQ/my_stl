// string header file
// contain the basic_string and char_traits

#ifndef _STRING_H_
#define _STRING_H_

#include "iterator"
#include "allocator.h"
#include "iterator.h"

namespace {

  // every char_type has different length
  // so, to coding char_traits is very pain!!!

  template<typename _ch>
  struct char_traits{
    using value_type = _ch;
    using char_pointer = _ch*;

    constexpr static size_t len();
    constexpr static int compare();
    constexpr static char_pointer copy();
    constexpr static char_pointer move();
    constexpr static char_pointer fill();

  };

  template<>
  struct char_traits<char>{
    using value_type = char;
    using char_pointer = value_type*;

    constexpr static size_t len();
    constexpr static int compare();
    constexpr static char_pointer copy();
    constexpr static char_pointer move();
    constexpr static char_pointer fill();

  };

  template<>
  struct char_traits<wchar_t>{
    using value_type = wchar_t;
    using char_pointer = value_type*;

    constexpr static size_t len();
    constexpr static int compare();
    constexpr static char_pointer copy();
    constexpr static char_pointer move();
    constexpr static char_pointer fill();
  };

  // if C++ 20: has char8_t
  // template<>
  // struct char_traits<char8_t>{
  //   using value_type = char8_t;
  // };

  template<>
  struct char_traits<char16_t>{
    using value_type = char16_t;
    using char_pointer = value_type*;

    constexpr static size_t len();
    constexpr static int compare();
    constexpr static char_pointer copy();
    constexpr static char_pointer move();
    constexpr static char_pointer fill();
  };

  template<>
  struct char_traits<char32_t>{
    using value_type = char32_t;
    using char_pointer = value_type*;

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
  using value_type      = typename data_allocator::value_type;
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

#endif // _STRING_H_
