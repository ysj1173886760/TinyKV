#include <gtest/gtest.h>
#include <random>
#include <map>
#include "memtable/rbtree.h"
#include "common/logger.h"

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
        data.emplace_back(dis(mt));
    }
    return data;
}

TEST(RBTreeTest, StrongTest) {
    RBTree<int, int> tree;
    const int MAX = 1e7;
    const int testSize = 1e5;

    std::vector<int> testData = getRandomData(testSize, -MAX, MAX);
    // LOG_DEBUG("start testing");

    std::map<int, int> mp;
    for (const auto &x : testData) {
        mp[x] = x * 2;
        tree.put(x, x * 2);
    }
    EXPECT_EQ(mp.size(), tree.size());

    // LOG_DEBUG("insertion complete");

    for (const auto &x : testData) {
        EXPECT_EQ(tree.get(x), x * 2);
    }

    // LOG_DEBUG("query complete");

    for (const auto &x : testData) {
        if (x > 0) {
            continue;
        }
        mp.erase(x);
        tree.remove(x);
    }

    EXPECT_EQ(mp.size(), tree.size());

    // LOG_DEBUG("remove complete");
    
    for (const auto &[key, val] : mp) {
        EXPECT_EQ(tree.get(key), val);
    }

    // LOG_DEBUG("query complete");
}

TEST(RBTreeTest, IteratorTest) {
    RBTree<int, int> tree;
    const int MAX = 1e7;
    const int testSize = 1e3;

    std::vector<int> testData = getRandomData(testSize, -MAX, MAX);

    std::map<int, int> mp;
    for (const auto &x : testData) {
        mp[x] = x * 2;
        tree.put(x, x * 2);
    }

    EXPECT_EQ(mp.size(), tree.size());
    std::vector<int> iterate_tree, iterate_map;
    for (const auto &x : tree) {
        iterate_tree.push_back(x.first);
    }
    for (const auto &x : mp) {
        iterate_map.push_back(x.first);
    }
    for (size_t i = 0; i < iterate_map.size(); i++) {
        EXPECT_EQ(mp[i], tree.get(i));
    }
}

}