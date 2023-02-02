#ifndef CONSTRUCT_H_
#define CONSTRUCT_H_

// 2022-12-24 (cpp) tangzheng


// construct : 构造
// destory   : 析构

#include <new>
#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER

#pragma waring(push)
#pragma waring(disable : 4100)

#endif

namespace tstl{

// construct
template <class _Ty>
void construct(_Ty* ptr){
    ::new (static_cast<void*>(ptr))_Ty();
}

// 这个在较新版的(>=201103L)的GCC版本中被遗弃了, 我也选择弃用
// template <class T1,typename T2>
// void construct(T1* ptr,const T2& value){
//     ::new (static_cast<void*>(ptr)) T2(value);
// }

//一个万能引用的实例
template <class T,typename... Args>
void construct(T* ptr,Args&&... args){
    ::new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
}

// destroy

// 用is_trivially_destructible 判断是否是可轻易破坏类型
// 利用模板判断传入的参数判断，自动选择适当的析构手段
// 如果是基本类型则什么都不做，自定义的复杂类型调用其析构

template <class ForwardIter>
void destroy(ForwardIter first,ForwardIter last);

template <class Ty>
void destroy(Ty* pointer);

template <class Ty>
void destroy_one(Ty*,std::true_type) {}

template <class Ty>
void destroy_one(Ty* pointer,std::false_type){
    if(pointer != nullptr)
    {
        pointer->~Ty();
    }
    // 这里 GCC C++17 用了个巨尼玛抽象的写法
}

template <class ForwardIter>
void destroy_cat(ForwardIter, ForwardIter,std::true_type){}

// If iterator than cast to regular pointer
template <class ForwardIter>
void destroy_cat(ForwardIter first,ForwardIter last,std::false_type){
    for(;first != last; ++first){
        destroy(&(*first));   
    }
}

// regular pointer
template <class Ty>
void destroy(Ty* pointer){
    destroy_one(pointer,std::is_trivially_destructible<Ty>{});
}

// iterator 
template <class ForwardIter>
void destroy(ForwardIter first,ForwardIter last){
    destroy_cat(first,last,std::is_trivially_destructible<
                    typename iterator_traits<ForwardIter>::value_type
                 >{});

}


}// end tstl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // end CONSTRUCT_H_
