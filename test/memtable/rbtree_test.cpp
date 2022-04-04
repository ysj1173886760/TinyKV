#include <gtest/gtest.h>
#include <random>
#include <map>
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
        if (i != 7) {
            EXPECT_EQ(tree.get(i), i * 2);
        } else {
            EXPECT_EQ(tree.get(i), 0);
        }
    }
}

std::vector<int> getRandomData(int testSize, 
                               int lb,
                               int ub) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dis(lb, ub);

    std::vector<int> data;
    for (int i = 0; i < testSize; i++) {
        // data.emplace_back(dis(mt));
        data.push_back(i);
    }
    return data;
}

TEST(RBTreeTest, StrongTest) {
    RBTree<int, int> tree;
    const int MAX = 1e7;
    const int testSize = 1e2;

    std::vector<int> testData = getRandomData(testSize, -MAX, MAX);

    std::map<int, int> mp;
    for (const auto &x : testData) {
        mp[x] = x * 2;
        tree.put(x, x * 2);
    }
    EXPECT_EQ(mp.size(), tree.size());

    for (const auto &x : testData) {
        EXPECT_EQ(tree.get(x), x * 2);
    }

    for (const auto &x : testData) {
        if (x > 0) {
            continue;
        }
        mp.erase(x);
        tree.remove(x);
    }
    EXPECT_EQ(mp.size(), tree.size());
    
    for (const auto &[key, val] : mp) {
        EXPECT_EQ(tree.get(key), val);
    }
}

}