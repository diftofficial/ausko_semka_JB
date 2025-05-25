#pragma once

#include <string>
#include "SimpleList.h"

template <typename Key, typename Value>
class SimpleMap {
public:
    struct Pair {
        Key key;
        Value value;
    };

    SimpleMap() {}
    ~SimpleMap() {}
    
    void insert(const Key& key, const Value& value) {
        for (size_t i = 0; i < data_.size(); ++i) {
            if (data_[i].key == key) {
                data_[i].value = value;
                return;
            }
        }
        data_.push_back({ key, value });
    }

    bool tryFind(const Key& key, Value*& out) {
        for (size_t i = 0; i < data_.size(); ++i) {
            if (data_[i].key == key) {
                out = &data_[i].value;
                return true;
            }
        }
        return false;
    }

    bool tryFind(const Key& key, Value*& out) const {
        for (size_t i = 0; i < data_.size(); ++i) {
            if (data_[i].key == key) {
                out = const_cast<Value*>(&data_[i].value);  // povolíme návrat pointeru aj v const kontexte
                return true;
            }
        }
        return false;
    }


    Value* get(const Key& key) {
        for (size_t i = 0; i < data_.size(); ++i) {
            if (data_[i].key == key) {
                return &data_[i].value;
            }
        }
        return nullptr;
    }

    void clear() { data_.clear(); }
    size_t size() const { return data_.size(); }

    // Iterácia cez páry
    Pair* begin() { return data_.begin(); }
    Pair* end() { return data_.end(); }
    const Pair* begin() const { return data_.begin(); }
    const Pair* end() const { return data_.end(); }

    


private:
    SimpleList<Pair> data_;
};
