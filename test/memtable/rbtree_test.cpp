#include <gtest/gtest.h>
#include "memtable/rbtree.h"

namespace TinyKV {

TEST(RBTreeTest, BasicTest) {
    RBTree<int, int> tree;
    EXPECT_EQ(tree.empty(), true);
    EXPECT_EQ(tree.size(), 0);

    for (int i = 1; i <= 10; i++) {
        tree.put(i, i * 2);
        EXPECT_EQ(tree.empty(), false);
        EXPECT_EQ(tree.size(), i);
    }
    tree.remove(7);
    EXPECT_EQ(tree.empty(), false);
    EXPECT_EQ(tree.size(), 9);
    for (int i = 10; i >= 1; i--) {
        EXPECT_EQ(tree.get(i), i * 2);
    }
}

}