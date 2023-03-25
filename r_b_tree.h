/**
 * 实现一个底层的红黑树
 */
#ifndef _R_B_TREE_H_
#define _R_B_TREE_H_

template <typename Tp>
struct Node{
  Node* parent;
  Node* left;
  Node* right;
  Tp m_data;
};

template <typename _Alloc>
struct tree_traits{
  using value_type = typename _Alloc::value_type;
  using pointer = typename _Alloc::pointer;
  using const_pointer = typename _Alloc::const_pointer;
};

#endif // _R_B_TREE_H_
