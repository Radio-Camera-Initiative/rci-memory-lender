#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

TEST(Concurrency, ThreadIntSeesBufferChanges) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::change_buffer_threaded<int>(r3, 5);
}

TEST(Concurrency, ThreadFloatSeesBufferChanges) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::change_buffer_threaded<float>(r3, 5.0);
}

TEST(Concurrency, ThreadFComplexSeesBufferChanges) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex c = 1if;
    unit_test::change_buffer_threaded<fcomplex>(r3, c);
}

TEST(Concurrency, ThreadIntMultiBufferChange) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    int data[3] = {5, 9, 12};
    unit_test::multi_change_buffer_threaded<int>(r3, data);
}

TEST(Concurrency, ThreadFloatMultiBufferChange) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    float data[3] = {5.0, 9.0, 12.0};
    unit_test::multi_change_buffer_threaded<float>(r3, data);
}

TEST(Concurrency, ThreadFComplexMultiBufferChange) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex data[3] = {5.f, 9if, 6.f - 6if};
    unit_test::multi_change_buffer_threaded<fcomplex>(r3, data);
}

TEST(Concurrency, ThreadIntWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::wait_on_fill_threaded<int>(r3, max, 5);
}

TEST(Concurrency, ThreadFloatWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::wait_on_fill_threaded<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadFComplexWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex c = 5.f - 5if;
    unit_test::wait_on_fill_threaded<fcomplex>(r3, max, c);
}

TEST(Concurrency, ThreadIntWaitsForQueue) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::buffer_from_empty_queue_threaded<int>(r3, max, 5);
}

TEST(Concurrency, ThreadFloatWaitsForQueue) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::buffer_from_empty_queue_threaded<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadFComplexWaitsForQueue) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex c = 5.f - 5if;
    unit_test::buffer_from_empty_queue_threaded<fcomplex>(r3, max, c);
}

TEST(Concurrency, ThreadIntMultiWaitsForFill) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::wait_multi_take_from_fill_threaded<int>(r3, max, 5);
}

TEST(Concurrency, ThreadFloatMultiWaitsForFill) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::wait_multi_take_from_fill_threaded<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadFComplexMultiWaitsForFill) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex c = 5.f - 5if;
    unit_test::wait_multi_take_from_fill_threaded<fcomplex>(r3, max, c);
}

TEST(Concurrency, ThreadWatchesBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::watcher_check_reference_counts<int>(r3);
}