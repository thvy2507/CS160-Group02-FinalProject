#ifndef CORE_DYNAMIC_ARRAY_H
#define CORE_DYNAMIC_ARRAY_H

// Day la file mock de compile test_master_data.cpp tren nhanh TV2
// Khi merge vao main, se su dung DynamicArray.h thuc te cua TV1 (Vy)

template <typename T>
class DynamicArray {
private:
    T* data_;
    int size_;
    int capacity_;

    void reallocate(int newCapacity) {
        T* newData = new T[newCapacity];
        for (int i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    DynamicArray() : size_(0), capacity_(10) {
        data_ = new T[capacity_];
    }
    ~DynamicArray() { delete[] data_; }

    void pushBack(const T& element) {
        if (size_ == capacity_) reallocate(capacity_ * 2);
        data_[size_++] = element;
    }

    int getSize() const { return size_; }

    T& operator[](int index) { return data_[index]; }
    const T& operator[](int index) const { return data_[index]; }
};

#endif // CORE_DYNAMIC_ARRAY_H
