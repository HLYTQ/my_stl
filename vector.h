// vector header

/*  来源是 msvc
    GCC 的版本，说实话太难看了！！！
*/

#ifndef VECTOR_H_
#define VECTOR_H_

#include "iterator.h"
#include "allocator.h"
#include "util.h"
#include "algorithm.h"
#include <initializer_list>

namespace tstl{

template <class _Myvec>
class __vec_const_iterator{
public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = typename _Myvec::value_type;
    using pointer           = typename _Myvec::const_pointer;
    using reference         = typename _Myvec::const_reference;
    using difference_type   = typename _Myvec::difference_type;

    pointer m_ptr;

public:
    constexpr __vec_const_iterator() = default;

    constexpr __vec_const_iterator(pointer ptr)
        : m_ptr(ptr) { }

    constexpr __vec_const_iterator& operator++() noexcept {
        ++m_ptr;
        return *this;
    }

    constexpr __vec_const_iterator operator++(int) {
        __vec_const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr __vec_const_iterator& operator--() {
        --m_ptr;
        return *this;
    }

    constexpr __vec_const_iterator operator--(int) {
        __vec_const_iterator tmp = *this;
        --*this;
        return tmp;
    }

    constexpr reference operator[](size_t index)
    { return *(m_ptr + index); }

    constexpr pointer operator->() 
    { return m_ptr; }

    constexpr reference operator*() const
    { return *m_ptr; }

    constexpr bool operator==(const __vec_const_iterator& other) 
    { return m_ptr == other.m_ptr; }
    
    constexpr bool operator!=(const __vec_const_iterator other)
    { return !(*this == other); }

    bool operator<(const __vec_const_iterator& _Right) const noexcept {
        return m_ptr < _Right.m_ptr;
    }

    bool operator>(const __vec_const_iterator& _Right) const noexcept {
        return _Right < *this;
    }

    bool operator<=(const __vec_const_iterator& _Right) const noexcept {
        return !(_Right < *this);
    }

    bool operator>=(const __vec_const_iterator& _Right) const noexcept {
        return !(*this < _Right);
    }


    constexpr __vec_const_iterator& operator+=(const difference_type off) {
        m_ptr += off;
        return *this;
    }
    
    constexpr __vec_const_iterator operator+(const difference_type off) const {
        __vec_const_iterator tmp = *this;
        tmp += off;
        return tmp;
    }

    constexpr __vec_const_iterator& operator-=(const difference_type off) {
        m_ptr -= off;
        return *this;
    }
    
    constexpr __vec_const_iterator operator-(const difference_type off) const {
        __vec_const_iterator tmp = *this;
        tmp -= off;
        return tmp;
    }

    constexpr difference_type operator-(const __vec_const_iterator& right) const {
        return m_ptr - right.m_ptr;
    }

}; // end __vec_const_iterator


template <class _Myvec>
constexpr __vec_const_iterator<_Myvec> operator+(
    typename __vec_const_iterator<_Myvec>::difference_type _Off, __vec_const_iterator<_Myvec> _Next) noexcept {
    return _Next += _Off;
}

template <class _Myvec>
struct iterator_traits<__vec_const_iterator<_Myvec>> {
    using iterator_category = random_access_iterator_tag;
    using pointer           = __vec_const_iterator<_Myvec>;
    using value_type        = const typename pointer::value_type;
    using difference_type   = typename pointer::difference_type;
};

// 这个应该修改成继承 __vec_const_iterator 的版本，这才是标准库给的设计
// 已修改，需要验证
template <class _Myvec>
class __vec_iterator : public __vec_const_iterator<_Myvec> {
public:
    using my_base = __vec_const_iterator<_Myvec>;

    using iterator_category = random_access_iterator_tag;
    using value_type        = typename _Myvec::value_type;
    using pointer           = typename _Myvec::pointer;
    using reference         = typename _Myvec::reference;
    using difference_type   = typename _Myvec::difference_type;

    constexpr __vec_iterator() = default;

    constexpr __vec_iterator(pointer ptr) 
        : __vec_const_iterator<_Myvec>(ptr) { }

    constexpr reference operator*() const {
        return const_cast<reference>(my_base::operator*());;
    }

    constexpr pointer operator->()
    { return this->m_ptr; }

    constexpr reference operator[](const difference_type index )
    { return const_cast<reference>(my_base::operator[](index)); }
    
    constexpr __vec_iterator& operator++() {
        my_base::operator++();
        return *this;
    }
    
    constexpr __vec_iterator operator++(int) {
        __vec_iterator tmp = *this;
        my_base::operator++();
        return tmp;
    }

    constexpr __vec_iterator& operator--() {
        my_base::operator--();
        return *this;
    }
    
    constexpr __vec_iterator operator--(int) {
        __vec_iterator tmp = *this;
        my_base::operator--();
        return tmp;
    }

    constexpr __vec_iterator& operator+=(const difference_type off) {
        my_base::operator+=(off);
        return *this;
    }

    constexpr __vec_iterator operator+(const difference_type off) const {
        __vec_iterator tmp = *this;
        tmp += off; 
        return tmp;
    }

    constexpr __vec_iterator& operator-=(const difference_type off) {
        my_base::oparator-=(off);
        return *this;
    }

    constexpr __vec_iterator operator-(const difference_type off) const {
        __vec_iterator tmp = *this;
        tmp -= off; 
        return tmp;
    }
  
       //pointer m_ptr;
}; // end __vec_iterator

template <class _Myvec>
constexpr __vec_iterator<_Myvec> operator+(
    typename __vec_iterator<_Myvec>::difference_type _Off, __vec_iterator<_Myvec> _Next) noexcept {
    return _Next += _Off;
}


template <class _Myvec>
struct iterator_traits<__vec_iterator<_Myvec>> {
    using iterator_category = random_access_iterator_tag;
    using pointer           = __vec_iterator<_Myvec>;
    using value_type        = typename pointer::value_type;
    using difference_type   = typename pointer::difference_type;
};

template <typename T, typename _Alloc = tstl::allocator<T>>
class vector {
public:
    using allocater_type = _Alloc;

    using data_allocater = _Alloc;
    
    using value_type       = typename allocater_type::value_type;
    using pointer          = typename allocater_type::pointer;
    using const_pointer    = typename allocater_type::const_pointer;
    using reference        = typename allocater_type::reference;
    using const_reference  = typename allocater_type::const_reference;
    using size_type        = typename allocater_type::size_type;
    using difference_type  = ptrdiff_t;
    
    using iterator               = __vec_iterator<vector>;
    using const_iterator         = __vec_const_iterator<vector>;
    using reverse_iterator       = typename tstl::reverse_iterator<iterator>;
    using reverse_const_iterator = typename tstl::reverse_iterator<const_pointer>;

public:

    constexpr allocater_type get_allocater()
    { return data_allocater(); }

    // construct function and destroy function:

    constexpr vector()
    { try_init(); }

    constexpr vector(const size_type count, const value_type& value)
    { construct_n_copies(count, value); }

    constexpr vector(std::initializer_list<T> list)
    { ranger_construct(list.begin(), list.end()); }

    constexpr vector(const vector& other){
        data_allocater::allocate(other.m_capacity);
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        for(size_type i = 0; i < other.size; ++i){
            data_allocater::construct(&m_data[i], other.m_data[i]);
        }
    }

    constexpr vector(vector&& other)
        :m_data(other.m_data),
         m_size(other.m_size),
         m_capacity(other.m_capacity)
    {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    constexpr vector& operator=(const vector& other){
        assert(other != *this);
        data_allocater::allocate(other.m_capacity);
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        for(size_type i = 0; i < other.size; ++i) {
            data_allocater::construct(&m_data[i], other.m_data[i]);
        }
        return *this;
    }

    constexpr vector& operator=(vector&& other) {
        assert(other != *this);
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

    ~vector() 
    { _Alloc::destroy(m_data, m_data + m_capacity); }

    // iterator below:

    constexpr iterator begin()
    { return iterator(m_data); }

    constexpr const_iterator begin() const 
    { return const_iterator(m_data); }

    constexpr iterator end()
    { return iterator(m_data + m_size); }

    constexpr const_iterator end() const
    { return const_iterator(m_data + m_size); }

    constexpr reverse_iterator rbegin() 
    { return reverse_iterator(end()); }

    constexpr reverse_const_iterator rbegin() const
    { return reverse_const_iterator(end()); }

    constexpr reverse_iterator rend() 
    { return reverse_iterator(begin()); }

    constexpr reverse_const_iterator rend() const 
    { return reverse_const_iterator(begin()); }

    // some basic function below: 

    constexpr void push_back(const value_type& value){
        if(m_size > m_capacity)
            realloc(m_capacity + m_capacity/2);
        
        data_allocater::construct(m_data + m_size, value);
        ++m_size;
    }
    
    constexpr void push_back(value_type&& value) {
        if(m_size > m_capacity)
            realloc(m_capacity + m_capacity/2);
        
        data_allocater::construct(m_data + m_size, tstl::move(value));
        ++m_size;
    }
    
    // c++14
    template<typename ...Args>
    constexpr decltype(auto) emplace_back(Args&& ...args) {
        if(m_size > m_capacity)
            realloc(m_capacity + m_capacity/2);

        data_allocater::construct(m_data + m_size, tstl::forward<Args>(args)...);
        
        return m_data[m_size++];
    }

    constexpr void pop_back() {
        if( m_size > 0)
            data_allocater::destroy(&m_data[--m_size]);
    }

    constexpr size_type size() const 
    { return static_cast<size_type>(m_size); }

    constexpr size_type capacity() const
    { return static_cast<size_type>(m_capacity); }

    constexpr bool empty() const
    { return m_size == 0; }

    constexpr void clear() {
        data_allocater::destroy(m_data, m_data + m_size);
        m_size = 0;
        m_capacity = 0;
    }

    constexpr void reserve(const size_type n) {
        if( capacity() < n){
            realloc(n);
        }
    }

    constexpr void resize(size_type newSize, const value_type& value);

    constexpr iterator erase(const_iterator where){
        pointer xpos = m_data + (where - begin());
        pointer end_(m_data + m_size);
        tstl::move(xpos + 1, end_, xpos);
        data_allocater::destroy(end_ - 1);
        m_size--;
        return xpos;
    }

    //constexpr iterator erase(const_iterator first, const_iterator last);

    // some visit function below

    constexpr const_reference operator[](const size_t index) const {
        assert(index < m_size);
        return m_data[index];
    }

    constexpr reference operator[](const size_t index) {
        assert(index < m_size);
        return m_data[index];
    }

    constexpr reference at(const size_type index) {
        assert(index < m_size);
        return (*this)[index];
    }

    constexpr const_reference at(const size_type index) const {
        assert(index < m_size);
        return (*this)[index];
    }

private:
    // help function below

    void realloc(const size_t newCapacity) noexcept { 
        pointer newBlock = data_allocater::allocate(newCapacity);
        
        if(newCapacity < m_size) 
            m_size = newCapacity;
        
        for(size_t i =  0; i < m_size; ++i)
            tstl::construct(newBlock + i, m_data[i]);
        
        tstl::destroy(m_data, m_data + m_size);

        m_data = newBlock;
        m_capacity = newCapacity;
    }

    constexpr void try_init() noexcept {
        m_data = data_allocater::allocate(2);
        m_capacity = 2;
    }

    constexpr void construct_n_copies(size_type count, const value_type& value){
        realloc(count);
        m_size = count;
        while(count > 0){
            data_allocater::construct(&m_data[count - 1], value);
            count--;
        }
    }

    template <class Iter>
    constexpr void ranger_construct(Iter first, Iter second){
        try_init();
        while (first != second) {
            emplace_back(*first);
            ++first;
        }
    }

private:
    pointer m_data = nullptr;
    size_type m_size = 0;
    size_type m_capacity = 0;
};


} // end namespace tst

#endif // end VECTOR_H_
