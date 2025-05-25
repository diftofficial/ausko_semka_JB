#pragma once

#include <cstddef>

template <typename T>
class SimpleList {
public:
    // Copy constructor
        SimpleList(const SimpleList<T>&other)
        : data_(nullptr), size_(other.size_), capacity_(other.capacity_)
    {
        if (capacity_ > 0) {
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
    }

    // Copy assignment operator
    SimpleList<T>& operator=(const SimpleList<T>& other)
    {
        if (this == &other) return *this; // ochrana proti samokópii

        // Vymažeme staré dáta
        delete[] data_;

        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = nullptr;

        if (capacity_ > 0) {
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    bool operator!=(const SimpleList<T>& other) const;
    bool operator==(const SimpleList<T>& other) const;
    SimpleList() : data_(nullptr), size_(0), capacity_(0) {}
    ~SimpleList() { delete[] data_; }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 4 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    T& operator[](size_t idx) { return data_[idx]; }
    const T& operator[](size_t idx) const { return data_[idx]; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    void clear() { size_ = 0; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }

private:
    void reserve(size_t newCap) {
        T* newData = new T[newCap];
        for (size_t i = 0; i < size_; ++i)
            newData[i] = data_[i];
        delete[] data_;
        data_ = newData;
        capacity_ = newCap;
    }
    T* data_;
    size_t size_;
    size_t capacity_;
};

template<typename T>
inline bool SimpleList<T>::operator!=(const SimpleList<T>& other) const
{
    return !(*this == other);
}

template<typename T>
inline bool SimpleList<T>::operator==(const SimpleList<T>& other) const

{
    return this->size() == other.size(); // jednoduché porovnanie podľa veľkost
}
