#ifndef ALGOBASE_H_
#define ALGOBASE_H_

#include "iterator.h"
#include "util.h"
#include <cstring>

/* 此文件的算法繁琐而多，许多直接复制了事了 */

namespace tstl{

// max 
template <class T>
const T& max(const T& a, const T& b)
{
    return a < b ? a : b;
}

template <class T, class Compare>
const T& max(const T& a, const T& b, Compare comp)
{
    return comp(a,b) ? a : b;
}

template <class T>
const T& min(const T& a, const T& b)
{
    return a > b ? a : b;
}

template <class T, class Compare>
const T& min(const T& a, const T& b, Compare comp)
{
    return comp(a,b) ? a : b;
}

template <class FIter1, class FIter2>
void iter_swap(FIter1 lhs, FIter2 rhs)
{
    tstl::swap(*lhs, *rhs);
}

// use two iterator first and last, copy the resource to result
template <class InputIter, class OutputIter>
OutputIter
uncheck_copy_cat(InputIter first, InputIter last, OutputIter result,
                 tstl::input_iterator_tag)
{
    for(; first != last; ++first, ++result){
        *result = *first;
    }
    return result;
}

template <class RandomIter, class OutputIter>
OutputIter
unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result,
                   tstl::random_access_iterator_tag)
{
  for(auto n = last - first; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

template <class InputIter, class OutputIter>
OutputIter
unchecked_copy(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_copy_cat(first, last, result, iterator_category(first));
}

template <class Tp, class Up>
typename std::enable_if<
  std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
  std::is_trivially_copy_assignable<Up>::value,Up*
>::type
unchecked_copy(Tp* first, Tp* last, Up* result)
{
  const auto n = static_cast<size_t>(last - first);
  if(n != 0)
    std::memmove(result, first, n * sizeof(Up));
  return result + n;
}

template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_copy(first, last ,result);
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
uncheck_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                          BidirectionalIter2 result, tstl::bidirectional_iterator_tag)
{
  while(first != last)
    *--result = *--last;
  return result;
}

template <class RandomIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
                            BidirectionalIter2 result, tstl::random_access_iterator_tag)
{
  for(auto n = last - first; n > 0; --n)
    *--result = *--last;
  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 
unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                        BidirectionalIter2 result)
{
  return unchecked_copy_backward_cat(first, last, result, iterator_category(first));
}

template <class Tp, class Up>
typename std::enable_if<
  std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
  std::is_trivially_copy_assignable<Up>::value, Up*>::type
unchecked_copy_backward(Tp* first, Tp* last, Up* result)
{
  const auto n = static_cast<size_t>(last - first);
  if(n != 0)
  {
    result -= n;
    std::memmove(result, first, n * sizeof(Up));
  } 
  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
  return nuchecked_copy_backwart(first, last, result);
}

template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter 
copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
{
  for(; first != last; ++first)
  {
    if(unary_pred(*first))
      *result++ = *first;
  }
  return result;
}

template <class InputIter, class Size, class OutputIter>
tstl::pair<InputIter, OutputIter>
unchecked_copy_n(InputIter first, 
                 Size n, 
                 OutputIter result, 
                 tstl::input_iterator_tag)
{
  for (; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return tstl::pair<InputIter, OutputIter>(first, result);
}

template <class RandomIter, class Size, class OutputIter>
tstl::pair<RandomIter, OutputIter>
unchecked_copy_n(RandomIter first, Size n, OutputIter result, 
                 tstl::random_access_iterator_tag)
{
  auto last = first + n;
  return tstl::pair<RandomIter, OutputIter>(last, tstl::copy(first, last, result));
}

template <class InputIter, class Size, class OutputIter>
tstl::pair<InputIter, OutputIter> 
copy_n(InputIter first, Size n, OutputIter result)
{
  return unchecked_copy_n(first, n, result, iterator_category(first));
}

template <class InputIter, class OutputIter>
OutputIter 
unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
                   tstl::input_iterator_tag)
{
  for (; first != last; ++first, ++result)
  {
    *result = tstl::move(*first);
  }
  return result;
}

// ramdom_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter 
unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
                   tstl::random_access_iterator_tag)
{
  for (auto n = last - first; n > 0; --n, ++first, ++result)
  {
    *result = tstl::move(*first);
  }
  return result;
}

template <class InputIter, class OutputIter>
OutputIter 
unchecked_move(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_move_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
  std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
  std::is_trivially_move_assignable<Up>::value,
  Up*>::type
unchecked_move(Tp* first, Tp* last, Up* result)
{
  const size_t n = static_cast<size_t>(last - first);
  if (n != 0)
    std::memmove(result, first, n * sizeof(Up));
  return result + n;
}

template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_move(first, last, result);
}

/*****************************************************************************************/
// move_backward
// 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
/*****************************************************************************************/
// bidirectional_iterator_tag 版本
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                            BidirectionalIter2 result, tstl::bidirectional_iterator_tag)
{
  while (first != last)
    *--result = tstl::move(*--last);
  return result;
}

// random_access_iterator_tag 版本
template <class RandomIter1, class RandomIter2>
RandomIter2
unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
                            RandomIter2 result, tstl::random_access_iterator_tag)
{
  for (auto n = last - first; n > 0; --n)
    *--result = tstl::move(*--last);
  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last, 
                        BidirectionalIter2 result)
{
  return unchecked_move_backward_cat(first, last, result,
                                     iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
  std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
  std::is_trivially_move_assignable<Up>::value,
  Up*>::type
unchecked_move_backward(Tp* first, Tp* last, Up* result)
{
  const size_t n = static_cast<size_t>(last - first);
  if (n != 0)
  {
    result -= n;
    std::memmove(result, first, n * sizeof(Up));
  }
  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
  return unchecked_move_backward(first, last, result);
}

/*****************************************************************************************/
// equal
// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    if (*first1 != *first2)  
      return false;
  }
  return true;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compared>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared comp)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    if (!comp(*first1, *first2))  
      return false;
  }
  return true;
}

/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
{
  for (; n > 0; --n, ++first)
  {
    *first = value;
  }
  return first;
}

// 为 one-byte 类型提供特化版本
template <class Tp, class Size, class Up>
typename std::enable_if<
  std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
  !std::is_same<Tp, bool>::value &&
  std::is_integral<Up>::value && sizeof(Up) == 1,
  Tp*>::type
unchecked_fill_n(Tp* first, Size n, Up value)
{
  if (n > 0)
  {
    std::memset(first, (unsigned char)value, (size_t)(n));
  }
  return first + n;
}

template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value)
{
  return unchecked_fill_n(first, n, value);
}

/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/
template <class ForwardIter, class T>
void fill_cat(ForwardIter first, ForwardIter last, const T& value,
              tstl::forward_iterator_tag)
{
  for (; first != last; ++first)
  {
    *first = value;
  }
}

template <class RandomIter, class T>
void fill_cat(RandomIter first, RandomIter last, const T& value,
              tstl::random_access_iterator_tag)
{
  fill_n(first, last - first, value);
}

template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value)
{
  fill_cat(first, last, value, iterator_category(first));
}

/*****************************************************************************************/
// lexicographical_compare
// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
// (1)如果第一序列的元素较小，返回 true ，否则返回 false
// (2)如果到达 last1 而尚未到达 last2 返回 true
// (3)如果到达 last2 而尚未到达 last1 返回 false
// (4)如果同时到达 last1 和 last2 返回 false
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2)
  {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
  }
  return first1 == last1 && first2 != last2;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compred>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2, Compred comp)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2)
  {
    if (comp(*first1, *first2))
      return true;
    if (comp(*first2, *first1))
      return false;
  }
  return first1 == last1 && first2 != last2;
}

// 针对 const unsigned char* 的特化版本
bool lexicographical_compare(const unsigned char* first1,
                             const unsigned char* last1,
                             const unsigned char* first2,
                             const unsigned char* last2)
{
  const auto len1 = last1 - first1;
  const auto len2 = last2 - first2;
  // 先比较相同长度的部分
  const auto result = std::memcmp(first1, first2, tstl::min(len1, len2));
  // 若相等，长度较长的比较大
  return result != 0 ? result < 0 : len1 < len2;
}

/*****************************************************************************************/
// mismatch
// 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
tstl::pair<InputIter1, InputIter2> 
mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
{
  while (first1 != last1 && *first1 == *first2)
  {
    ++first1;
    ++first2;
  }
  return tstl::pair<InputIter1, InputIter2>(first1, first2);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compred>
tstl::pair<InputIter1, InputIter2> 
mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp)
{
  while (first1 != last1 && comp(*first1, *first2))
  {
    ++first1;
    ++first2;
  }
  return tstl::pair<InputIter1, InputIter2>(first1, first2);
}

}// end namespace tstl


#endif
