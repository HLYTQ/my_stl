/**
 * 实现一个底层的红黑树
 */
#ifndef _R_B_TREE_H_
#define _R_B_TREE_H_

#include "type_traits.hpp"
#include "iterator.hpp"
#include "allocator.hpp"
#include <cstddef>

namespace tstl{

enum Colors {
    color_red,
    color_black
};

template <typename Tp>
class rb_tree_node;

template <typename Tp>
struct rb_tree_node_base {
    rb_tree_node<Tp>* parent = nullptr;
    rb_tree_node<Tp>* left = nullptr;
    rb_tree_node<Tp>* right = nullptr;
    Colors color;
};

template <typename Tp>
class rb_tree_node : public rb_tree_node_base<Tp> {
private:
    Tp value;
};

template <typename NodePtr>
inline void set_black(NodePtr& node) noexcept {
    node->color = color_black;
}

template <typename NodePtr>
inline void set_red(NodePtr& node) noexcept {
    node->color = color_red;
}

template <typename NodePtr>
inline bool is_red(NodePtr& node) noexcept {
    return node->color == color_red;
}

template <typename NodePtr>
inline bool is_black(NodePtr& node) noexcept {
    return node->color == color_black;
}

template <typename NodePtr>
inline bool is_left(NodePtr& node) noexcept {
    return node->parent->left == node;
}

template <typename NodePtr>
inline NodePtr get_grandParent(NodePtr node) {
    return node->parent->parent;
}

template <typename NodePtr>
inline NodePtr get_uncle(NodePtr node) {
    if(node->parent == get_grandParent(node)->left)
        return get_grandParent(node)->right;
    else 
        return get_grandParent(node)->left;
}

/*---------------------------------------*\
|       p                         p       |
|      / \                       / \      |
|     x   d    rotate left      y   d     |
|    / \       ===========>    / \        |
|   a   y                     x   c       |
|      / \                   / \          |
|     b   c                 a   b         |
\*---------------------------------------*/
template <typename NodePtr>
void rotate_left(NodePtr x, NodePtr& root) noexcept {
    NodePtr y = x->right;
    x->right = y->left;
    y->parent = x->parent;

    if(x == root){
        root = y;
    }else if(is_left(x)){
        x->parent->left = y;

    }else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}


template <typename NodePtr>
void rotate_right(NodePtr x, NodePtr& root) noexcept {
    NodePtr y = x->left;
    x->left = y->right;
    y->parent = x->parent;

    if(x == root){
        root = y;
    }else if (is_left(x)) {
        x->parent->left = y;
    }else {
        x->parent->rigth = y;
    }
    y->right = x;
    x->parent = y;
}

// case 1: 新增节点位于根节点，令新增节点为黑
// case 2: 新增节点的父节点为黑，没有破坏平衡，直接返回
// case 3: 父节点和叔叔节点都为红，令父节点和叔叔节点为黑，祖父节点为红，
//         然后令祖父节点为当前节点，继续处理
// case 4: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为右（左）孩子，
//         让父节点成为当前节点，再以当前节点为支点左（右）旋
// case 5: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为左（右）孩子，
//         让父节点变为黑色，祖父节点变为红色，以祖父节点为支点右（左）旋
// https://www.bilibili.com/video/BV1jf4y1M7XM?p=42&vd_source=53f17d97d3e6f3320ea1669222833ce7+
template <typename NodePtr> void insert_reblance(NodePtr x, NodePtr &root) {
    set_red(x);
    // case 3:
    while (x != root && is_red(x->parent)) {
        NodePtr uncle = get_uncle(x);
        if (rb_tree_is_lchild(x->parent)) {
          if (uncle != nullptr && is_red(uncle)) {
            set_black(x->parent);
            set_black(uncle);
            x = x->parent->parent;
            set_red(x);
          } else {
            // case 4:
            if (!is_left(x)) {
              x = x->parent;
              rotate_left(x, root);
            }
            // case 5:
            set_black(x->parent);
            set_red(x->parent->parent);
            rotate_right(x->parent->parent, root);
            break;
          }
        // 对称处理
        } else {
          if (uncle != nullptr && is_red(uncle)) {
            set_black(x->parent);
            set_black(uncle);
            x = x->parent->parent;
            set_red(x);
          } else {
            // case 4:
            if (!is_left(x)) {
              x = x->parent;
              rotate_right(x, root);
            }
            // case 5:
            set_black(x->parent);
            set_red(x->parent->parent);
            rotate_left(x->parent->parent, root);
            break;
          }
        }
    }
    set_black(root);
}





} // namespace tstl

#endif // _R_B_TREE_H_
