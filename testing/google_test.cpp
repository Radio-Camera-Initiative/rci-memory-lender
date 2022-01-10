#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../r3.hpp"

TEST(BasicFunctions, MakeRecycleMemory) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    unit_test::make_recycle_memory(shape, max);
}

TEST(BasicFunctions, TakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::take_one_buffer_from_fill(r3, shape, max);
}

TEST(BasicFunctions, CheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    unit_test::check_buffer_destruction(r3, max);
}

TEST(BasicFunctions, ChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::change_one_buffer(r3, max, 5);
}

TEST(BasicFunctions, MultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::multi_change_buffer(r3, max, 5);
}

TEST(BasicFunctions, QueueBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::queue_buffer_from_fill(r3, max);
}

TEST(Concurrency, ThreadSeesBufferChanges) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::change_buffer_threaded(r3);
}

TEST(Concurrency, ThreadMultiBufferChange) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::multi_change_buffer_threaded(r3);
}

TEST(Concurrency, ThreadWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::wait_take_from_fill_threaded(r3, max);
}

TEST(Concurrency, ThreadWaitsForQueue) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::buffer_from_empty_queue_threaded(r3, max);
}

TEST(Concurrency, ThreadMultiWaitsForFill) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::wait_multi_take_from_fill_threaded(r3, max);
}

TEST(Concurrency, ThreadWatchesBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::watcher_check_reference_counts(r3);
}