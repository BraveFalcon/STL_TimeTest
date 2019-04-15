#pragma once

#include <cstddef>
#include <cmath>
#include <algorithm>
#include <chrono>

class TimeTest {
private:
    size_t prev1_num_elems = 0;
    size_t prev2_num_elems = 0;
    double prev1_time_per_run = 0;
    double prev2_time_per_run = 0;

    virtual void prepare_for_test(size_t num_elems, size_t num_runs) = 0;


    virtual void clear_memory() = 0;

    virtual void run_test() = 0;

    virtual void run_waste() = 0;

    double get_test_time() {
        auto start = std::chrono::high_resolution_clock::now();
        run_test();
        auto stop = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
    }

    double get_waste_time() {
        auto start = std::chrono::high_resolution_clock::now();
        run_waste();
        auto stop = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
    }

    size_t get_num_runs(size_t num_elems, double time) {
        if (prev1_num_elems != 0)
            return static_cast<size_t>(std::max(1.0,
                                                time / (prev2_time_per_run + (prev1_time_per_run - prev2_time_per_run)
                                                                             * (num_elems - prev2_num_elems) /
                                                                             (prev1_num_elems - prev2_num_elems))));

        size_t cur_num_runs = 1;
        prepare_for_test(num_elems, cur_num_runs);
        double cur_time = get_test_time();
        clear_memory();
        double eps = 0.05;
        if (cur_time > (1 - eps) * time)
            return 1;
        while (std::abs(cur_time - time) / time > eps) {
            cur_num_runs = static_cast<size_t>(cur_num_runs * time / cur_time);
            prepare_for_test(num_elems, cur_num_runs);
            cur_time = get_test_time();
            clear_memory();
        }
        return cur_num_runs;
    }

public:
    double test(size_t num_elems, double time) {
        size_t num_runs = get_num_runs(num_elems, time);
        prepare_for_test(num_elems, num_runs);
        double test_time = get_test_time();
        double waste_time = get_waste_time();
        clear_memory();
        prev2_num_elems = prev1_num_elems;
        prev2_time_per_run = prev1_time_per_run;
        prev1_num_elems = num_elems;
        prev1_time_per_run = test_time / num_runs;
        return (test_time - waste_time)  / num_runs / num_elems;
    }
};
