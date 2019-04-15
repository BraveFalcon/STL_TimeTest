#pragma once

#include "TimeTest.hpp"

template<typename CONTAINER>
class SequentialAccessTest : public TimeTest {
private:
    CONTAINER *container = nullptr;
    size_t num_elems = 0;
    size_t num_runs = 0;


    void prepare_for_test(size_t num_elems, size_t num_runs) override {
        container = new CONTAINER(num_elems);
        this->num_elems = num_elems;
        this->num_runs = num_runs;
    }

    void clear_memory() override {
        delete container;
    }

    void run_test() override {
        volatile auto elem = (*container)[0];
        for (size_t test = 0; test < num_runs; ++test)
            for (size_t i = 0; i < num_elems; ++i)
                elem = (*container)[i];
    }

    void run_waste() override {
        volatile size_t elem = 0;
        for (size_t test = 0; test < num_runs; ++test)
            for (size_t i = 0; i < num_elems; ++i)
                elem = i;
    }

public:
    SequentialAccessTest() = default;

    ~SequentialAccessTest() {
        clear_memory();
    }
};