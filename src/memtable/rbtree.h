/**
 * @file rbtree.h
 * @author sheep
 * @brief red black tree
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef RBTREE_H
#define RBTREE_H

#include <functional>
#include "memtable.h"

namespace TinyKV {

#define IS_LS(x) ((x) != nullptr && (x)->_fa->_ls == (x))
#define IS_RS(x) ((x) != nullptr && (x)->_fa->_rs == (x))

template< class KeyType, class ValueType, class Compare = std::less<KeyType> >
class RBTree: public Memtable<KeyType, ValueType, Compare> {
private:
    using _Base = Memtable<KeyType, ValueType, Compare>;
    using _Base::_comparator;

    struct Node {
        KeyType _key;
        ValueType _value;
        bool _red;
        Node *_fa;
        Node *_ls;
        Node *_rs;
        int _size;

        Node(KeyType key = KeyType(),
             ValueType value = ValueType(),
             bool red = true,
             Node *fa = nullptr,
             Node *ls = nullptr,
             Node *rs = nullptr,
             int size = 1
             ): 
            _key(key),
            _value(value),
            _red(red),
            _fa(fa),
            _ls(ls),
            _rs(rs),
            _size(size) {}

        Node *leftNode() {
            Node *cur = this;
            if (!_ls) {
                while (cur->_fa && cur->_fa->_ls == cur) {
                    cur = cur->_fa;
                }
                cur = cur->_fa;
            } else {
                cur = cur->_ls;
                while (cur->_rs) {
                    cur = cur->_rs;
                }
            }
            return cur;
        }

        Node *rightNode() {
            Node *cur = this;
            if (!_rs) {
                while (cur->_fa && cur->_fa->_rs == cur) {
                    cur = cur->_fa;
                }
                cur = cur->_fa;
            } else {
                cur = cur->_rs;
                while (cur->_ls) {
                    cur = cur->_ls;
                }
            }
            return cur;
        }

        void maintain() {
            _size = 1;
            if (_ls) {
                _size += _ls->_size;
            }
            if (_rs) {
                _size += _rs->_size;
            }
        }

        Node *getBrother() {
            return _fa->_ls == this ? _fa->_rs : _fa->_ls;
        }

        Node *getSucc() {
            Node *cur = _rs;
            while (cur->_ls != nullptr) {
                cur->_size--;
                cur = cur->_ls;
            }
            return cur;
        }
    };

    Node *_root;
    Node *_hot;

    void destroy(Node *cur);
    void update(Node *cur);
    void connect34(Node *root, Node *rootLs, Node *rootRs, Node *tree1, Node *tree2, Node *tree3, Node *tree4);
    void solveDoubleRed(Node *cur);
    void solveDoubleBlack(Node *cur);
    Node *rfind(const KeyType &key, const int op = 0);
    Node *getFirstElement();

public:
    RBTree(const Compare &comp = Compare()): _Base(comp), _root(nullptr), _hot(nullptr) {}
    ~RBTree() {
        destroy(_root);
    }

    struct iterator;

    ValueType get(const KeyType &key);
    void put(const KeyType &key, ValueType value);
    void remove(const KeyType &key);
    int size();
    bool empty();

    iterator begin() {
        return iterator(getFirstElement());
    }
    iterator end() {
        return iterator();
    }
};

template< class KeyType, class ValueType, class Compare>
struct RBTree<KeyType, ValueType, Compare>::iterator {
private:
    Node *_node;

public:
    iterator& operator--() {
        _node = _node->leftNode();
        return *this;
    }

    iterator& operator++() {
        _node = _node->rightNode();
        return *this;
    }

    std::pair<KeyType, ValueType> operator*() {
        return std::make_pair(_node->_key, _node->_value);
    }

    Node *operator->() {
        return _node;
    }

    iterator(Node *node = nullptr): _node(node) {}
    iterator(const iterator &iter): _node(iter._node) {}

    bool operator!=(const iterator &iter) {
        return iter._node != _node;
    }
};

}

#endif