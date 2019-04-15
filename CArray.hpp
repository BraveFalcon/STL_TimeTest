#pragma once

#include <cstddef>

template<typename T>
class CArray {
private:
    T *array;
public:
    explicit CArray(size_t num_elems) {
        array = new T[num_elems];
    }

    ~CArray() {
        delete[] array;
    }

    T &operator[](size_t i) {
        return array[i];
    }
};