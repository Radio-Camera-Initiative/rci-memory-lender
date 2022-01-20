#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../r3.hpp"

TEST(BasicFunctions, IntMakeRecycleMemory) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    unit_test::make_recycle_memory<int>(shape, max);
}

TEST(BasicFunctions, IntTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::take_one_buffer_from_fill<int>(r3, shape, max);
}

TEST(BasicFunctions, IntCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    unit_test::check_buffer_destruction<int>(r3, max);
}

TEST(BasicFunctions, IntChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::change_one_buffer<int>(r3, max, 5);
}

TEST(BasicFunctions, IntMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::multi_change_buffer<int>(r3, max, 5, 2);
}

TEST(BasicFunctions, IntSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::set_buffer_ptr_array<int>(r3);
}

TEST(BasicFunctions, IntQueueBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::queue_buffer_from_fill<int>(r3, max, 5);
}

TEST(BasicFunctions, FloatMakeRecycleMemory) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    unit_test::make_recycle_memory<float>(shape, max);
}

TEST(BasicFunctions, FloatTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::take_one_buffer_from_fill<float>(r3, shape, max);
}

TEST(BasicFunctions, FloatCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    unit_test::check_buffer_destruction<float>(r3, max);
}

TEST(BasicFunctions, FloatChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::change_one_buffer<float>(r3, max, 5.0);
}

TEST(BasicFunctions, FloatMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::multi_change_buffer<float>(r3, max, 5.0, 2.0);
}

TEST(BasicFunctions, FloatSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::set_buffer_ptr_array<float>(r3);
}

TEST(BasicFunctions, FloatQueueBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::queue_buffer_from_fill<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadIntSeesBufferChanges) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::change_buffer_threaded<int>(r3, 5);
}

TEST(Concurrency, ThreadFloatSeesBufferChanges) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::change_buffer_threaded<float>(r3, 5.0);
}

TEST(Concurrency, ThreadIntMultiBufferChange) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    int data[3] = {5, 9, 12};
    unit_test::multi_change_buffer_threaded<int>(r3, data);
}

TEST(Concurrency, ThreadFloatMultiBufferChange) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    float data[3] = {5.0, 9.0, 12.0};
    unit_test::multi_change_buffer_threaded<float>(r3, data);
}

TEST(Concurrency, ThreadIntWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::wait_take_from_fill_threaded<int>(r3, max, 5);
}

TEST(Concurrency, ThreadFloatWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::wait_take_from_fill_threaded<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadIntWaitsForQueue) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::buffer_from_empty_queue_threaded<int>(r3, max, 5);
}

TEST(Concurrency, ThreadFloatWaitsForQueue) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::buffer_from_empty_queue_threaded<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadIntMultiWaitsForFill) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::wait_multi_take_from_fill_threaded<int>(r3, max, 5);
}

TEST(Concurrency, ThreadFloatMultiWaitsForFill) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::wait_multi_take_from_fill_threaded<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadWatchesBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::watcher_check_reference_counts<int>(r3);
}