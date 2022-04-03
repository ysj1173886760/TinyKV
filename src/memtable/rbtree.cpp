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

#include <iostream>
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
    Node *cur = rfind(key, 0);
    if (cur != nullptr) {
        cur->_value = value;
        return;
    }

    if (!_hot) {
        _root = new Node(key, value, false, nullptr, nullptr, nullptr, 1);
        return;
    }

    cur = new Node(key, value, true, _hot, nullptr, nullptr, 1);
    if (_comparator(_hot->_key, key)) {
        _hot->_rs = cur;
    } else {
        _hot->_ls = cur;
    }
    update(cur);
    solveDoubleRed(cur);
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::solveDoubleRed(Node *cur) {
    while (cur->_fa == nullptr || cur->_fa->_red) {
        if (cur == _root) {
            cur->_red = false;
            return;
        }

        Node *fa = cur->_fa;
        if (!fa->_red) {
            return;
        }

        Node *uncle = fa->getBrother();
        Node *grdfa = fa->_fa;
        if (uncle != nullptr && uncle->_red) {
            grdfa->_red = true;
            uncle->_red = false;
            fa->_red = false;
            cur = grdfa;
        } else {
            if (IS_LS(fa)) {
                if (IS_LS(cur)) {
                    // ll
                    fa->_fa = grdfa->_fa;
                    if (grdfa == _root) {
                        _root = fa;
                    } else if (grdfa->_fa->_ls == grdfa) {
                        grdfa->_fa->_ls = fa;
                    } else {
                        grdfa->_fa->_rs = fa;
                    }

                    connect34(fa, cur, grdfa, cur->_ls, cur->_rs, fa->_rs, uncle);
                    fa->_red = false;
                    grdfa->_red = true;
                } else {
                    // lr
                    if (grdfa == _root) {
                        _root = cur;
                    } else if (grdfa->_fa->_ls == grdfa) {
                        grdfa->_fa->_ls = cur;
                    } else {
                        grdfa->_fa->_rs = cur;
                    }

                    connect34(cur, fa, grdfa, fa->_ls, cur->_ls, cur->_rs, uncle);
                    cur->_red = false;
                    grdfa->_red = true;
                }
            } else {
                if (IS_LS(cur)) {
                    // rl
                    cur->_fa = grdfa->_fa;
                    if (grdfa == _root) {
                        _root = cur;
                    } else if (grdfa == grdfa->_fa->_ls) {
                        grdfa->_fa->_ls = cur;
                    } else {
                        grdfa->_fa->_rs = cur;
                    }

                    connect34(cur, grdfa, fa, uncle, cur->_ls, cur->_rs, fa->_rs);
                    cur->_red = false;
                    grdfa->_red = true;
                } else {
                    // rr
                    fa->_fa = grdfa->_fa;
                    if (grdfa == _root) {
                        _root = fa;
                    } else if (grdfa == grdfa->_fa->_ls) {
                        grdfa->_fa->_ls = fa;
                    } else {
                        grdfa->_fa->_rs = fa;
                    }

                    connect34(fa, grdfa, cur, uncle, fa->_ls, cur->_ls, cur->_rs);
                    fa->_red = false;
                    grdfa->_red = true;
                }
            }
            return;
        }
    }
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::update(Node *cur) {
    while (cur) {
        cur->maintain();
        cur = cur->_fa;
    }
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::connect34(Node *root, 
                                                    Node *rootLs, 
                                                    Node *rootRs, 
                                                    Node *tree1, 
                                                    Node *tree2, 
                                                    Node *tree3, 
                                                    Node *tree4) {
    rootLs->_ls = tree1;
    if (tree1) {
        tree1->_fa = rootLs;
    }
    rootLs->_rs = tree2;
    if (tree2) {
        tree2->_fa = rootLs;
    }
    
    rootRs->_ls = tree3;
    if (tree3) {
        tree3->_fa = rootRs;
    }
    rootRs->_rs = tree4;
    if (tree4) {
        tree4->_fa = rootRs;
    }
    root->_ls = rootLs;
    root->_rs = rootRs;
    rootLs->_fa = root;
    rootRs->_fa = root;
    rootLs->maintain();
    rootRs->maintain();
    root->maintain();
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::solveDoubleBlack(Node *cur) {
    while (cur != _root) {
        Node *fa = cur->_fa;
        Node *brother = cur->getBrother();
        if (brother->_red) {
            brother->_red = false;
            fa->_red = true;
            if (fa == _root) {
                _root = brother;
            }

            if (fa->_fa) {
                if (fa->_fa->_ls == fa) {
                    fa->_fa->_ls = brother;
                } else {
                    fa->_fa->_rs = brother;
                }
            }
            brother->_fa = fa->_fa;
            if (IS_LS(cur)) {
                connect34(brother, fa, brother->_rs, cur, brother->_ls, brother->_rs->_ls, brother->_rs->_rs);
            } else {
                connect34(brother, brother->_ls, fa, brother->_ls->_ls, brother->_ls->_rs, brother->_rs, cur);
            }
            brother = cur->getBrother();
            fa = cur->_fa;
        }

        if (brother->_ls && brother->_ls->_red) {
            bool oldColor = fa->_red;
            fa->_red = false;
            if (fa->_ls == cur) {
                if (fa->_fa) {
                    if (fa->_fa->_ls == fa) {
                        fa->_fa->_ls = brother->_ls;
                    } else {
                        fa->_fa->_rs = brother->_rs;
                    }
                }
                brother->_ls->_fa = fa->_fa;
                if (fa == _root) {
                    _root = brother->_ls;
                }
                connect34(brother->_ls, fa, brother, cur, brother->_ls->_ls, brother->_ls->_rs, brother->_rs);
            } else {
                brother->_ls->_red = false;
                if (fa->_fa) {
                    if (fa->_fa->_ls == fa) {
                        fa->_fa->_ls = brother;
                    } else {
                        fa->_fa->_rs = brother;
                    }
                }
                brother->_fa = fa->_fa;
                if (fa == _root) {
                    _root = brother;
                }
                connect34(brother, brother->_ls, fa, brother->_ls->_ls, brother->_ls->_rs, brother->_rs, cur);
            }
            fa->_fa->_red = oldColor;
            return;
        } else if (brother->_rs && brother->_rs->_red) {
            bool oldColor = fa->_red;
            fa->_red = false;
            if (fa->_ls == cur) {
                brother->_rs->_red = false;
                if (fa->_fa) {
                    if (fa->_fa->_ls == fa) {
                        fa->_fa->_ls = brother;
                    } else {
                        fa->_fa->_rs = brother;
                    }
                }
                brother->_fa = fa->_fa;
                if (fa == _root) {
                    _root = brother;
                }
                connect34(brother, fa, brother->_rs, cur, brother->_ls, brother->_rs->_ls, brother->_rs->_rs);
            } else {
                if (fa->_fa) {
                    if (fa->_fa->_ls == fa) {
                        fa->_fa->_ls = brother->_rs;
                    } else {
                        fa->_fa->_rs = brother->_rs;
                    }
                }
                brother->_rs->_fa = fa->_fa;
                if (fa == _root) {
                    _root = brother->_rs;
                }
                connect34(brother->_rs, brother, fa, brother->_ls, brother->_rs->_ls, brother->_rs->_rs, cur);
            }
            fa->_fa->_red = oldColor;
            return;
        }

        if (fa->_red) {
            fa->_red = false;
            brother->_red = true;
            return;
        }

        brother->_red = true;
        cur = fa;
    }
}

template< class KeyType, class ValueType, class Compare >
ValueType RBTree<KeyType, ValueType, Compare>::get(const KeyType &key) {
    Node *cur = rfind(key);
    if (!cur) {
        return ValueType();
    }
    return cur->_value;
}

template< class KeyType, class ValueType, class Compare >
int RBTree<KeyType, ValueType, Compare>::size() {
    return _root ? _root->_size : 0;
}

template< class KeyType, class ValueType, class Compare >
bool RBTree<KeyType, ValueType, Compare>::empty() {
    return !_root;
}

template< class KeyType, class ValueType, class Compare >
void RBTree<KeyType, ValueType, Compare>::remove(const KeyType &key) {
    Node *cur = rfind(key);
    if (!cur) {
        return;
    }

    Node *node_suc;
    while (cur->_ls || cur->_rs) {
        if (!cur->_ls) {
            node_suc = cur->_rs;
        } else if (!cur->_rs) {
            node_suc = cur->_ls;
        } else {
            node_suc = cur->getSucc();
        }
        cur->_key = node_suc->_key;
        cur->_value = node_suc->_value;
        cur = node_suc;
    }
    cur->_size = 0;
    update(cur->_fa);
    if (!cur->_red) {
        solveDoubleBlack(cur);
    }

    if (cur == _root) {
        _root = nullptr;
        delete cur;
        return;
    }

    if (cur->_fa->_ls == cur) {
        cur->_fa->_ls = nullptr;
    } else {
        cur->_fa->_rs = nullptr;
    }
    delete cur;
    return;
}

}

int main() {
    TinyKV::RBTree<int, int> tree;
    std::cout << tree.empty() << " " << tree.size() << std::endl;
    for (int i = 1; i <= 10; i++) {
        tree.put(i, i * 2);
        std::cout << tree.empty() << " " << tree.size() << std::endl;
    }
    tree.remove(7);
    std::cout << tree.empty() << " " << tree.size() << std::endl;
    for (int i = 10; i >= 1; i--) {
        std::cout << tree.get(i) << std::endl;
    }
}

#endif