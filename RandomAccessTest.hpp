#pragma once

#include "TimeTest.hpp"
#include <random>

template<typename CONTAINER>
class RandomAccessTest : public TimeTest {
private:
    CONTAINER *container = nullptr;
    const int *rand_nums;
    unsigned int *indexes = nullptr;
    size_t num_elems = 0;
    size_t num_runs = 0;


    void prepare_for_test(size_t num_elems, size_t num_runs) override {
        container = new CONTAINER(num_elems);
        indexes = new unsigned int[num_elems];
        for (size_t i = 0; i < num_elems; ++i)
            indexes[i] = static_cast<unsigned int>(std::abs(rand_nums[i]) % num_elems);
        this->num_elems = num_elems;
        this->num_runs = num_runs;
    }

    void clear_memory() override {
        delete[] indexes;
        delete container;
    }

    void run_test() override {
        volatile auto elem = (*container)[0];
        for (size_t test = 0; test < num_runs; ++test)
            for (size_t i = 0; i < num_elems; ++i)
                elem = (*container)[indexes[i]];
    }

    void run_waste() override {
        volatile auto elem = indexes[0];
        for (size_t test = 0; test < num_runs; ++test)
            for (size_t i = 0; i < num_elems; ++i)
                elem = indexes[i];
    }

public:
    explicit RandomAccessTest(const int *rands) : rand_nums(rands) {}

    ~RandomAccessTest() {
        clear_memory();
    }
};