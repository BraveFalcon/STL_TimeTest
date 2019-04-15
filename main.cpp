#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include "RandomAccessTest.hpp"
#include "CArray.hpp"
#include "SequentialAccessTest.hpp"

#define TYPE int
const size_t MIN_SIZE = 256;
const size_t MAX_SIZE = 2L * 1024 * 1024 * 1024 / sizeof(TYPE) + 1;
const double STEP_FACTOR = 1.3;
const double TIME_PER_TEST = 0.5;


int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "You forgot to write outfile's name";
        exit(1);
    }
    FILE *outfile = fopen(argv[1], "w");
    if (!outfile) {
        std::cerr << "Can't create file: " << argv[1] << std::endl;
        exit(1);
    }

    int num_steps = static_cast<int>(std::log(MAX_SIZE / MIN_SIZE) / std::log(STEP_FACTOR));
    std::cout << "Number of steps: " << num_steps << std::endl;
    printf("Full test time: %.0f s\n", 3 * num_steps * TIME_PER_TEST);
    printf("Generating random data...\n");
    /*
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis;
    auto rands = new int[MAX_SIZE];
    for (size_t i = 0; i < MAX_SIZE; ++i)
        rands[i] = dis(gen);
    */

    SequentialAccessTest<std::vector<TYPE>> vector_test;
    SequentialAccessTest<std::deque<TYPE>> deque_test;
    SequentialAccessTest<CArray<TYPE>> array_test;

    fprintf(outfile, "ints\tc-array\tvector\tdeque\n");
    size_t num_elems = MIN_SIZE;
    for (int i = 0; i < num_steps; ++i) {
        printf("\rComplete: %.0f%%", 100.0 * i / num_steps);
        std::cout.flush();
        double arr_time = array_test.test(num_elems, TIME_PER_TEST);
        double vec_time = vector_test.test(num_elems, TIME_PER_TEST);
        double deq_time = deque_test.test(num_elems, TIME_PER_TEST);
        fprintf(outfile, "%zu\t%.3e\t%.3e\t%.3e\n", num_elems, arr_time, vec_time, deq_time);
        fflush(outfile);
        num_elems = size_t(num_elems * STEP_FACTOR);
    }
    std::cout << std::endl;
    //delete[] rands;
}