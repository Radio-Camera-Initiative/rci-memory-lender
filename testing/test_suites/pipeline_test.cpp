#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

TEST(Pipeline, IntEqualThreadsBuffers) {
    int buffers = 5;
    int threads = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, buffers); 
    std::vector<int> filling(30);
    std::iota(std::begin(filling), std::end(filling), 0);
    unit_test::run_m_threads_n_buffers<int>(r3, filling, threads);
}

TEST(Pipeline, IntMoreThreads) {
    int buffers = 5;
    int threads = 10;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, buffers); 
    std::vector<int> filling(30);
    std::iota(std::begin(filling), std::end(filling), 0);
    unit_test::run_m_threads_n_buffers<int>(r3, filling, threads);
}

TEST(Pipeline, IntMoreBuffers) {
    int buffers = 10;
    int threads = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, buffers); 
    std::vector<int> filling(30);
    std::iota(std::begin(filling), std::end(filling), 0);
    unit_test::run_m_threads_n_buffers<int>(r3, filling, threads);
}