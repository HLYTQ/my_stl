/* cherno 在课程中的设计似乎是来源于 msvc 的，
 * 这里我实现的版本是偷的 GCC 的设计
 * 遗憾的是似乎 GCC 的写法抽象的厉害
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include "iterator.hpp"

namespace tstl{

template <typename _Tp,size_t N_m>
struct  __array_traits
{
    typedef _Tp _Type[N_m];

    static constexpr _Tp& S_ref(const _Type& t,size_t n) noexcept
    { return const_cast<_Tp&>(t[n]); }

    static constexpr _Tp* S_ptr(const _Type& t) noexcept
    { return const_cast<_Tp*>(t); }
};

template <typename _Tp>
struct  __array_traits<_Tp,0>
{
    struct _Type { };

    static constexpr _Tp& S_ref(const _Type t,size_t) noexcept
    { return *const_cast<_Tp*>(nullptr); }

    static constexpr _Tp* S_ptr(const _Type& t) noexcept
    { return nullptr; }
};


template <typename _Tp,size_t N_m>
struct array{
    typedef _Tp                  value_type;
    typedef value_type*          pointer;
    typedef const value_type*    const_pointer;
    typedef value_type&          reference;
    typedef const value_type&    const_reference;
    typedef value_type*          iterator;
    typedef const value_type*    const_iterator;
    typedef std::size_t          size_type;
    typedef std::ptrdiff_t       difference_type;

    typedef tstl::reverse_iterator<iterator> reverse_iterator;
    typedef tstl::reverse_iterator<const_iterator> const_reverse_iterator;

    // Support for zero-sized arrays mandator
    typedef tstl::__array_traits<_Tp, N_m> _AT_Type;
    // 实际的数组所在，C++绝对要将数组视为一个类型
    typename _AT_Type::_Type                m_elem;        

    array() = default;

    iterator begin() noexcept
    { return iterator(data()); }

    const_iterator begin() const noexcept
    { return const_iterator(data()); }

    iterator end() noexcept
    { return iterator(data() + N_m); }

    const_iterator end() const noexcept
    { return const_iterator(data() + N_m);}

    reverse_iterator
    rbegin() noexcept
    { return reverse_iterator(end()); }

    const_reverse_iterator
    rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator
    rend() noexcept
    { return reverse_iterator(begin()); }

    constexpr const_reverse_iterator
    rend() const noexcept
    { return const_reverse_iterator(begin()); }

    reference operator[](size_type index) noexcept
    { return _AT_Type::S_ref(m_elem,index);}

    constexpr const_reference 
    operator[] (size_type index) const noexcept
    { return _AT_Type::S_ref(m_elem,index);}

    pointer data() noexcept
    { return _AT_Type::S_ptr(m_elem); }

    constexpr const_pointer 
    data() const noexcept
    { return _AT_Type::S_ptr(m_elem); }

    constexpr size_type size() const noexcept
    { return N_m; }

    constexpr bool empty() const noexcept
    { return size() == 0; }
};

  
}// END namespace tstl

#endif // end ARRAY_H
