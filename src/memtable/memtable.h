/**
 * @file memtable.h
 * @author sheep
 * @brief base class for memtable
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <functional>

namespace TinyKV {

template< class KeyType, class ValueType, class Compare = std::less<KeyType> >
class Memtable {
protected:
    Compare _comparator;

public:
    Memtable(const Compare &comp = Compare()): _comparator(comp) {}
    virtual ~Memtable() {}

    virtual ValueType get(const KeyType &key) = 0;
    virtual void put(const KeyType &key, ValueType value) = 0;
    virtual void remove(const KeyType &key) = 0;
    virtual int size() = 0;
    virtual bool empty() = 0;
};

}

#endif