/**
 * @file main.cpp
 * @author sheep
 * @brief 
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include "memtable/rbtree.h"
#include "memtable/memtable.h"

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