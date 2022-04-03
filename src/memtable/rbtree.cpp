/**
 * @file rbtree.cpp
 * @author sheep
 * @brief red black tree implementation
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef RBTREE_CPP
#define RBTREE_CPP

#include "rbtree.h"

namespace TinyKV {

/**
 * @brief 
 * free the RBTree node recursively
 * @tparam KeyType 
 * @tparam ValueType 
 * @tparam Compare 
 * @param cur 
 */
template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::destroy(Node *cur) {
    if (!cur) {
        return;
    }
    destroy(cur->_ls);
    destroy(cur->_rs);
    delete cur;
}

template< class KeyType, class ValueType, class Compare >
typename RBTree<KeyType, ValueType, Compare>::Node*
RBTree<KeyType, ValueType, Compare>::rfind(const KeyType &key, const int op) {
    Node *cur = _root;
    _hot = nullptr;
    while (cur != nullptr && cur->_key != key) {
        _hot = cur;
        cur->_size += op;
        cur = (_comparator(key, cur->_key)) ? cur->_ls : cur->_rs;
    }
    return cur;
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::put(const KeyType &key, ValueType value) {
    // size doesn't change if we have already got the key
    Node *cur = rfind(key, 0);
    if (cur != nullptr) {
        cur->_value = value;
        return;
    }

    if (!_hot) {
        _root = new Node(key, value, false, nullptr, nullptr, nullptr, 1);
        return;
    }
    // otherwise, we need to increase the size
    // TODO: we are doing twice finding here, find a way to eliminate one.
    cur = rfind(key, 1);

    cur = new Node(key, value, true, _hot, nullptr, nullptr, 1);
    if (_comparator(_hot->_key, key)) {
        _hot->_rs = cur;
    } else {
        _hot->_ls = cur;
    }
    solveDoubleRed(cur);
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::solveDoubleRed(Node *cur) {
    while (cur->_fa == nullptr || cur->_fa->_color) {
        if (cur == _root) {
            cur->_color = false;
            return;
        }

        Node *fa = cur->_fa;
        if (!fa->_color) {
            return;
        }

        Node *uncle = fa->get_bro();
    }
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::update(Node *cur) {

}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::connect34(Node *root, 
                                                    Node *rootLs, 
                                                    Node *rootRs, 
                                                    Node *tree1, 
                                                    Node *tree2, 
                                                    Node *tree3, 
                                                    Node *tree4) {

}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::solveDoubleBlack(Node *cur) {
}

template< class KeyType, class ValueType, class Compare >
ValueType RBTree<KeyType, ValueType, Compare>::get(const KeyType &key) {
    return ValueType();
}

template< class KeyType, class ValueType, class Compare >
int RBTree<KeyType, ValueType, Compare>::size() {
    return 0;
}

template< class KeyType, class ValueType, class Compare >
bool RBTree<KeyType, ValueType, Compare>::empty() {
    return false;
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::remove(const KeyType &key) {
}

}

int main() {
    TinyKV::RBTree<int, int> tree;
    tree.put(1, 1);
}

#endif