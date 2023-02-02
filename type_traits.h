#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

//this used to get type information
//use standard header for type_traits


// it to much,so use this cpp std type_traits
#include <type_traits>

/* 2020-12-13 (cpp) tangzheng  */

namespace tstl
{

template <class T,T v>
    struct m_integral_constant
    {
        static constexpr T value = v;
    };
  
template <bool b>
    using m_bool_constant = m_integral_constant<bool,b>;

using m_true_type = m_bool_constant<true>;
using m_false_type = m_bool_constant<false>;

template<class T1,class T2>
    struct pair;

//这里是做了一些模板特化，写入的参数会在编译期匹配到相应的模板
//to check the type

//this is provide by my_tiny_stl's author
template<class T>
    struct is_pair 
    : public m_false_type{};

template<class T1,class T2>
    struct is_pair<pair<T1,T2>> 
    : public m_true_type{};

//and below is my

//use to remove const and reference
template<typename T>
    struct remove_const{
        using type = T;
    };

template<typename T>
    struct remove_const<const T>{
        using type = T;
    };

template<typename T>
    struct remove_reference{
        using type = T;
    };

template<typename T>
    struct remove_reference<T &>{
        using type = T;
    };

template<typename T>
    struct remove_reference<T &&>{
        using type = T;
    };

template<typename T>
    using remove_cr_t = typename remove_const<
                        typename remove_reference<T>::type
                        >::type;


template <typename>
    struct is_floating_point_helper
    : public m_false_type {};

template <>
    struct is_floating_point_helper<float> 
    : public m_true_type {};

template <>
    struct is_floating_point_helper<double> 
    : public m_true_type {};

template <>
    struct is_floating_point_helper<long double> 
    : public m_true_type {};

// 实际的is_floating_point

template <typename T>
struct is_floating_point
  : public is_floating_point_helper<remove_cr_t<T>>
  { };

template<typename>
    struct is_void_helper
    : public m_false_type { };

template<>
    struct is_void_helper<void>
    : public m_true_type { };

  /// is_void
template<typename T>
    struct is_void
    : public is_void_helper<remove_cr_t<T>>
    { };

template<typename>
    struct __is_integral_helper
    : public m_false_type { };

  template<>
    struct __is_integral_helper<bool>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<char>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<signed char>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<unsigned char>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<wchar_t>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<char16_t>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<char32_t>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<short>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<unsigned short>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<int>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<unsigned int>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<long>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<unsigned long>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<long long>
    : public m_true_type { };

  template<>
    struct __is_integral_helper<unsigned long long>
    : public m_true_type { };

  template<typename _Tp>
    struct is_integral
    : public __is_integral_helper<remove_cr_t<_Tp>>
    { };

//and other type like  array, point as well... if need than add them

  template <bool _Pre, typename _Tp = void>
    struct enable_if { };
  
  template <typename _Tp>
    struct enable_if<true, _Tp>{
      using type = _Tp;
    };  



}// namespace tstl


#endif
