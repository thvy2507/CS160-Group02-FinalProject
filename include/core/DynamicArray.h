
/**
 * @file DynamicArray.h
 * @author TV1
 * @brief Provides a dynamically resizing array without STL containers.
 */

#pragma once

#include <stdexcept>

template <typename T>
class DynamicArray {
public:
    // Constructors, Destructor & Copy Assignment
    explicit DynamicArray(int initialCapacity = 4)
        : m_data(nullptr), m_size(0), m_capacity(initialCapacity) {
        if (initialCapacity <= 0) {
            throw std::invalid_argument("Capacity must be positive");
        }
        m_data = new T[m_capacity];
    }

    DynamicArray(const DynamicArray& other)
        : m_data(new T[other.m_capacity]),
          m_size(other.m_size), m_capacity(other.m_capacity) {
        try {
            for (int i = 0; i < m_size; ++i) {
                m_data[i] = other.m_data[i];
            }
        } catch (...) {
            delete[] m_data;
            throw;
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) {
            return *this;
        }

        T* newData = new T[other.m_capacity];
        try {
            for (int i = 0; i < other.m_size; ++i) {
                newData[i] = other.m_data[i];
            }
        } catch (...) {
            delete[] newData;
            throw;
        }

        delete[] m_data;
        m_data = newData;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        return *this;
    }

    ~DynamicArray() {
        delete[] m_data;
    }

    // Modification Methods
    void add(const T& value) {
        T copiedValue = value;

        if (m_size == m_capacity) {
            grow();
        }

        m_data[m_size] = copiedValue;
        ++m_size;
    }

    void clear() {
        m_size = 0;
    }

    void remove(int index) {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of range");
        }

        for (int i = index; i < m_size - 1; ++i) {
            m_data[i] = m_data[i + 1];
        }

        --m_size;
    }

    // Accessors
    bool empty() const {
        return m_size == 0;
    }

    T& get(int index) {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }

    const T& get(int index) const {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }

    int getSize() const {
        return m_size;
    }

    int size() const {
        return m_size;
    }

    T& operator[](int index) {
        return get(index);
    }

    const T& operator[](int index) const {
        return get(index);
    }

private:
    // Private Helpers
    void grow() {
        int newCapacity = m_capacity * 2;
        T* newData = new T[newCapacity];

        try {
            for (int i = 0; i < m_size; ++i) {
                newData[i] = m_data[i];
            }
        } catch (...) {
            delete[] newData;
            throw;
        }

        delete[] m_data;
        m_data = newData;
        m_capacity = newCapacity;
    }

    // Private Members
    T* m_data;
    int m_size;
    int m_capacity;
};
