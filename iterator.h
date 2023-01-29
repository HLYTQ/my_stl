#ifndef ITERATOR_H_
#define ITERATOR_H_

/*
 *  <https://zhuanlan.zhihu.com/p/85810006>
 */

#include <cstddef>
#include "type_traits.h"

#define VERSION_C_OR_S 1 

namespace tstl{

// Five iterator types  ----tag
//     单向移动只读迭代器 Input Iterator
//     单向移动只写迭代器 Output Iterator
//     单向移动读写迭代器 Forward Iterator
//     双向移动读写迭代器 Bidirectional Iterator
//     不定序读写迭代器   Random_access Iterator

struct input_iterator_tag { };

struct output_iterator_tag { };

struct forward_iterator_tag
  : public input_iterator_tag { };

struct bidirectional_iterator_tag
  : public forward_iterator_tag { };

struct random_access_iterator_tag 
  : public bidirectional_iterator_tag { };

// iterator template
//     ptrdiff_t 设计用来保存两个指针之差

template < class Category,class T,
           class Distance = ptrdiff_t,
           class Pointer = T*,
           class Reference = T& >
struct iterator
{
  //迭代器的类型
  using iterator_category = Category;
  
  using value_type = T;
  using pointer = Pointer; 
  using reference = Reference;
  using difference_type = Distance; 
};

#if VERSION_C_OR_S

//????????????????
//传入迭代器类型，模板特化到char value = true
//不是则匹配到two value = false
template <class T>
struct has_iterator_category{
private:
  struct two { 
    char a, b;
  };
  template <class U> 
    static two test(...);
  template <class U> 
    static char test(typename U::iterator_category* = 0);
public:
  static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
};

// iterator traits
// If false ,do nothing.If true than do.
template <class I,bool>
struct iterator_traits_impl {};

template <class I>
struct iterator_traits_impl<I,true>{
  using iterator_category = typename I::iterator_category;
  using value_type        = typename I::value_type;
  using pointer           = typename I::pointer;
  using reference         = typename I::reference;
  using difference_type   = typename I::difference_type;
};

template <class I,bool>
struct iterator_traits_helper {};

template <class I>
  struct iterator_traits_helper<I,true>:public iterator_traits_impl<I,
      std::is_convertible<typename I::iterator_category,input_iterator_tag>::value || 
      std::is_convertible<typename I::iterator_category,output_iterator_tag>::value>
  { };

// all the things have been done above is:
// to check whether the tmplate 'class' is
// an iterator during the compile time

template <class I>
  struct iterator_traits
  : public iterator_traits_helper<I,has_iterator_category<I>::value> {};

// partial specialization for regular pointers
template <class T>
struct iterator_traits<T*>{
  using iterator_category = random_access_iterator_tag;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
};

// partial specialization for regular cosnt pointers
template <class T>
struct iterator_traits<const T*>{
  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using pointer = const T*;
  using reference = const T&;
  using difference_type = ptrdiff_t;
};

#else //version_complex

template<class I>
    struct iterator_traits{
        using iterator_category = typename I::iterator_category;
        using value_type        = typename I::value_type;
        using pointer           = typename I::pointer;
        using reference         = typename I::reference;
        using difference_type   = typename I::difference_type;
    };
    
// partial specialization for regular pointers
template <class T>
    struct iterator_traits<T*>{
      using iterator_category = random_access_iterator_tag;
      using value_type        = T;
      using pointer           = T*;
      using reference         = T&;
      using difference_type   = ptrdiff_t;
    };

// partial specialization for regular cosnt pointers
template <class T>
    struct iterator_traits<const T*>{
      using iterator_category = random_access_iterator_tag;
      using value_type = T;
      using pointer = const T*;
      using reference = const T&;
      using difference_type = ptrdiff_t;
    };


#endif // version_simple


// 当存在有iterator_category时用于判断是不是某一iterator类型
template <class T,class U,
          bool = has_iterator_category
            <iterator_traits<T>>::value>
  struct has_iterator_category_of
    : public m_bool_constant<
              std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value>
             { };

// 萃取某种迭代器

// 当不存在时直接为继承false_type
template <class T, class U>
    struct has_iterator_category_of<T, U, false> 
    : public m_false_type {};

template <class Iter>
    struct is_input_iterator 
    : public has_iterator_category_of<Iter, input_iterator_tag> {};

template <class Iter>
    struct is_output_iterator 
    : public has_iterator_category_of<Iter, output_iterator_tag> {};

template <class Iter>
    struct is_forward_iterator 
    : public has_iterator_category_of<Iter, forward_iterator_tag> {};

template <class Iter>
    struct is_bidirectional_iterator 
    : public has_iterator_category_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
    struct is_random_access_iterator 
    : public has_iterator_category_of<Iter, random_access_iterator_tag> {};

// 判断是否是迭代器
template <class Iterator>
    struct is_iterator 
    : public m_bool_constant<is_input_iterator<Iterator>::value ||
        is_output_iterator<Iterator>::value>
    { };

// 萃取某个迭代器的 category
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
  typedef typename iterator_traits<Iterator>::iterator_category Category;
  return Category();
}

// 萃取某个迭代器的 distance_type
template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取某个迭代器的 value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// distance of input_iterator_tag
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
__distance_dispatch(InputIterator first,InputIterator last,input_iterator_tag){
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last ) {
        ++first;
        ++n;
    }
    return n;
}

//特化版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type
__distance_dispatch(RandomIter first, RandomIter last,
        random_access_iterator_tag){
    return last - first;
}

//外部调用接口
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first,InputIterator last){
    return __distance_dispatch(first,last,iterator_category(first));
}

// 以下函数用于让迭代器前进 n 个距离

// advance 的 input_iterator_tag 的版本
template <class InputIterator, class Distance>
void __advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
{
  while (n--) 
    ++i;
}

// advance 的 bidirectional_iterator_tag 的版本
template <class BidirectionalIterator, class Distance>
void __advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
  if (n >= 0)
    while (n--)  ++i;
  else
    while (n++)  --i;
}

// advance 的 random_access_iterator_tag 的版本
template <class RandomIter, class Distance>
void __advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
{
  i += n;
}

// advance 的外部接口
template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n)
{
  __advance_dispatch(i, n, iterator_category(i));
}

// reverse_iterator
// 前进为后退，后退为前进
template<class Iterator>
class reverse_iterator{
private:
    Iterator current;
public:
    using iterator_category = typename iterator_traits<Iterator>::iterator_category;
    using value_type = typename iterator_traits<Iterator>::value_type;
    using difference_type = typename iterator_traits<Iterator>::difference_type;
    using pointer = typename iterator_traits<Iterator>::pointer;
    using reference = typename iterator_traits<Iterator>::reference;

    using iterator_type = Iterator;
    using self = reverse_iterator<Iterator>;
public:
  // 构造函数
  reverse_iterator() {}
  explicit reverse_iterator(iterator_type i) :current(i) {}
  reverse_iterator(const self& rhs) :current(rhs.current) {}

public:
  // 取出对应的正向迭代器
  iterator_type base() const 
  { return current; }

  // 重载操作符
  reference operator*() const
  { // 实际对应正向迭代器的前一个位置
    auto tmp = current;
    return *--tmp;
  }
  pointer operator->() const
  {
    return &(operator*());
  }

  // 前进(++)变为后退(--)
  self& operator++()
  {
    --current;
    return *this;
  }
  self operator++(int)
  {
    self tmp = *this;
    --current;
    return tmp;
  }
  // 后退(--)变为前进(++)
  self& operator--()
  {
    ++current;
    return *this;
  }
  self operator--(int)
  {
    self tmp = *this;
    ++current;
    return tmp;
  }

  self& operator+=(difference_type n)
  {
    current -= n;
    return *this;
  }
  self operator+(difference_type n) const
  {
    return self(current - n);
  }
  self& operator-=(difference_type n)
  {
    current += n;
    return *this;
  }
  self operator-(difference_type n) const
  {
    return self(current + n);
  }

  reference operator[](difference_type n) const
  {
    return *(*this + n);
  }
};

// 重载 operator-
template <class Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs,
          const reverse_iterator<Iterator>& rhs)
{
  return rhs.base() - lhs.base();
}

// 重载比较操作符
template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
  return lhs.base() == rhs.base();
}

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
  const reverse_iterator<Iterator>& rhs)
{
  return rhs.base() < lhs.base();
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
  return !(lhs == rhs);
}

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs)
{
  return rhs < lhs;
}

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
  return !(rhs < lhs);
}

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
  return !(lhs < rhs);
}

}// end tstl
#endif // end ITERATOR_H_
