#pragma once

#include <libds/amt/implicit_sequence.h>
#include <libds/adt/queue.h>
#include <libds/adt/array.h>
#include <functional>
#include <cmath>
#include <algorithm>
#include <type_traits>

namespace ds::adt {

    template <typename T>
    struct Sort {
    public:
        virtual void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) = 0;
        void sort(amt::ImplicitSequence<T>& is) {
            sort(is, [](const T& a, const T& b)->bool { return a < b; });
        }
    };

    //----------

    // Selection Sort
    template <typename T>
    class SelectSort : public Sort<T> {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            size_t n = is.size();
            for (size_t i = 0; i + 1 < n; ++i) {
                size_t minIdx = i;
                for (size_t j = i + 1; j < n; ++j) {
                    if (compare(is.access(j)->data, is.access(minIdx)->data))
                        minIdx = j;
                }
                std::swap(is.access(i)->data, is.access(minIdx)->data);
            }
        }
    };

    //----------

    // Insertion Sort
    template <typename T>
    class InsertSort : public Sort<T> {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            size_t n = is.size();
            for (size_t i = 1; i < n; ++i) {
                T tmp = is.access(i)->data;
                size_t j = i;
                while (j > 0 && compare(tmp, is.access(j - 1)->data)) {
                    is.access(j)->data = is.access(j - 1)->data;
                    --j;
                }
                is.access(j)->data = tmp;
            }
        }
    };

    //----------

    // Bubble Sort
    template <typename T>
    class BubbleSort : public Sort<T> {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            size_t n = is.size();
            if (n < 2) return;
            bool swapped;
            do {
                swapped = false;
                for (size_t i = 0; i + 1 < n; ++i) {
                    if (compare(is.access(i + 1)->data, is.access(i)->data)) {
                        std::swap(is.access(i + 1)->data, is.access(i)->data);
                        swapped = true;
                    }
                }
            } while (swapped);
        }
    };

    //----------

    // Quick Sort
    template <typename T>
    class QuickSort : public Sort<T> {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            if (!is.isEmpty())
                quick(is, compare, 0, is.size() - 1);
        }
    private:
        void quick(amt::ImplicitSequence<T>& is,
            std::function<bool(const T&, const T&)> compare,
            size_t low, size_t high)
        {
            T pivot = is.access(low + (high - low) / 2)->data_;
            size_t left = low, right = high;
            do {
                while (compare(is.access(left)->data_, pivot)) ++left;
                while (right > low && compare(pivot, is.access(right)->data_)) --right;
                if (left <= right) {
                    std::swap(is.access(left)->data_, is.access(right)->data_);
                    ++left;
                    if (right > low) --right;
                }
            } while (left <= right);
            if (low < right) quick(is, compare, low, right);
            if (left < high) quick(is, compare, left, high);
        }
    };

    //----------

    // Heap Sort
    template <typename T>
    class HeapSort : public Sort<T> {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            size_t n = is.size();
            // build max‐heap
            for (size_t i = 1; i < n; ++i) {
                size_t current = i;
                bool again;
                do {
                    again = false;
                    size_t parent = (current - 1) / 2;
                    if (current > 0 && compare(is.access(parent)->data, is.access(current)->data)) {
                        std::swap(is.access(current)->data, is.access(parent)->data);
                        current = parent;
                        again = true;
                    }
                } while (again);
            }
            // extract one by one
            for (size_t i = n - 1; i > 0; --i) {
                std::swap(is.access(0)->data, is.access(i)->data);
                size_t current = 0;
                bool again;
                do {
                    again = false;
                    size_t l = 2 * current + 1, r = 2 * current + 2;
                    size_t maxIdx;
                    if (l < i && r < i)
                        maxIdx = compare(is.access(r)->data, is.access(l)->data) ? l : r;
                    else
                        maxIdx = (l < i ? l : r);
                    if (maxIdx < i && compare(is.access(current)->data, is.access(maxIdx)->data)) {
                        std::swap(is.access(current)->data, is.access(maxIdx)->data);
                        current = maxIdx;
                        again = true;
                    }
                } while (again);
            }
        }
    };

    //----------

    // Shell Sort
    template <typename T>
    class ShellSort : public Sort<T> {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            size_t k = static_cast<size_t>(std::ceil(std::log10(is.size())));
            shell(is, compare, k);
        }
    private:
        void shell(amt::ImplicitSequence<T>& is,
            std::function<bool(const T&, const T&)> compare,
            size_t k)
        {
            size_t n = is.size();
            for (size_t d = 0; d < k; ++d) {
                for (size_t i = d; i < n; ++i) {
                    size_t j = i;
                    while (j >= k && j - k >= d &&
                        compare(is.access(j)->data, is.access(j - k)->data))
                    {
                        std::swap(is.access(j)->data, is.access(j - k)->data);
                        j -= k;
                    }
                }
            }
            if (k > 1) shell(is, compare, k - 1);
        }
    };

    //----------

    // Radix Sort (base-10 buckets)
    template <typename Key, typename T>
    class RadixSort : public Sort<T> {
        static_assert(std::is_integral_v<Key>, "Radix sort supports only integral types.");
    public:
        RadixSort() : getKey_([](auto const& x) { return x; }) {}
        RadixSort(std::function<Key(const T&)> getKey) : getKey_(getKey) {}

        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            size_t n = is.size();
            Array<ExplicitQueue<T>> buckets(10);
            for (size_t b = 0; b < 10; ++b)
                buckets[b] = ExplicitQueue<T>();

            Key component = 1;
            bool more = true;
            while (more) {
                more = false;
                // distribute
                for (size_t i = 0; i < n; ++i) {
                    Key k = getKey_(is.access(i)->data);
                    int idx = int((k / component) % 10);
                    buckets[idx].enqueue(is.access(i)->data);
                    if (k / (component * 10) > 0) more = true;
                }
                component *= 10;
                // collect
                size_t pos = 0;
                for (size_t b = 0; b < 10; ++b) {
                    while (!buckets[b].isEmpty()) {
                        is.access(pos++)->data = buckets[b].dequeue();
                    }
                }
            }
        }
    private:
        std::function<Key(const T&)> getKey_;
    };

    //----------

    // Merge Sort (bottom-up via queues)
    template <typename T>
    class MergeSort : public Sort<T> {
    public:
        void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override {
            size_t n = is.size();
            queue1_ = new ImplicitQueue<T>(n);
            queue2_ = new ImplicitQueue<T>(n);
            mergeQueue_ = new ImplicitQueue<T>(n);

            // enqueue all
            for (size_t i = 0; i < n; ++i)
                mergeQueue_->enqueue(is.access(i)->data);

            size_t run = 1;
            while (run < n) {
                split(run);
                merge(compare, run);
                run <<= 1;
            }
            split(run);
            merge(compare, run);

            // dequeue back to sequence
            for (size_t i = 0; i < n; ++i)
                is.access(i)->data = mergeQueue_->dequeue();

            delete queue1_;
            delete queue2_;
            delete mergeQueue_;
        }
    private:
        void split(size_t run) {
            size_t cnt = 0;
            bool first = true;
            while (!mergeQueue_->isEmpty()) {
                if (cnt % run == 0) { cnt = 0; first = !first; }
                if (first) queue1_->enqueue(mergeQueue_->dequeue());
                else       queue2_->enqueue(mergeQueue_->dequeue());
                ++cnt;
            }
        }

        void merge(std::function<bool(const T&, const T&)> compare, size_t run) {
            size_t c1 = 0, c2 = 0;
            while (!queue1_->isEmpty() || !queue2_->isEmpty()) {
                if (c1 == 0 && c2 == 0) {
                    c1 = std::min(run, queue1_->size());
                    c2 = std::min(run, queue2_->size());
                }
                T* k1 = c1 > 0 ? &queue1_->peek() : nullptr;
                T* k2 = c2 > 0 ? &queue2_->peek() : nullptr;
                if (k1 && k2) {
                    if (compare(*k1, *k2)) { --c1; mergeQueue_->enqueue(queue1_->dequeue()); }
                    else { --c2; mergeQueue_->enqueue(queue2_->dequeue()); }
                }
                else if (k1) { --c1; mergeQueue_->enqueue(queue1_->dequeue()); }
                else { --c2; mergeQueue_->enqueue(queue2_->dequeue()); }
            }
        }

        ImplicitQueue<T>* queue1_{ nullptr };
        ImplicitQueue<T>* queue2_{ nullptr };
        ImplicitQueue<T>* mergeQueue_{ nullptr };
    };

} // namespace ds::adt
