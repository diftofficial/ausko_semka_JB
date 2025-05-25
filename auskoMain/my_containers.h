#pragma once
#include <libds/adt/array.h>
#include <libds/adt/table.h>
#include <initializer_list>

// --- Náš List (dá sa použiť Array alebo ImplicitSequence) ---
template <typename T>
class MyList {
public:
    MyList() : arr_(10) {}
    MyList(std::initializer_list<T> lst) : arr_(lst.size()) {
        size_t i = 0;
        for (auto& el : lst) arr_.set(el, i++);
    }

    void push_back(const T& val) { arr_.set(val, arrSize_++); }
    size_t size() const { return arrSize_; }
    auto begin() { return arr_.begin(); }
    auto end() { return arr_.begin() + arrSize_; }
    auto begin() const { return arr_.begin(); }
    auto end() const { return arr_.begin() + arrSize_; }
    void insert(typename MyList<T>::iterator, T a, T b) {} // fake

private:
    ds::adt::Array<T> arr_;
    size_t arrSize_ = 0;
};



// --- Náš Map-like kontajner (kľúče sú enum, môže byť pole!) ---
template <typename Key, typename Val>
class MyMap {
public:
    MyMap() { for (int i = 0; i < 8; ++i) used_[i] = false; }

    Val& operator[](Key k) {
        int i = static_cast<int>(k);
        used_[i] = true;
        return arr_[i];
    }

    auto find(Key k) const {
        int i = static_cast<int>(k);
        return used_[i] ? &arr_[i] : nullptr;
    }
    auto begin() { return arr_; }
    auto end() { return arr_ + 8; }
    void clear() { for (int i = 0; i < 8; ++i) used_[i] = false; }

private:
    Val arr_[8]; // 8 for safety
    bool used_[8];
};

