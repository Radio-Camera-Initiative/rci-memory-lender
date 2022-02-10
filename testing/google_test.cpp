#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../r3.hpp"
#include <complex>

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

TEST(BasicFunctions, IntDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::dec_buffer_ref_count<int>(r3);
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

TEST(BasicFunctions, IntDecOperate) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max);
    unit_test::dec_operate_queue<int>(r3);
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

TEST(BasicFunctions, FloatDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::dec_buffer_ref_count<float>(r3);
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

TEST(BasicFunctions, FloatDecOperate) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max);
    unit_test::dec_operate_queue<float>(r3);
}

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

TEST(BasicFunctions, FComplexMakeRecycleMemory) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    unit_test::make_recycle_memory<fcomplex>(shape, max);
}

TEST(BasicFunctions, FComplexTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    unit_test::take_one_buffer_from_fill<fcomplex>(r3, shape, max);
}

TEST(BasicFunctions, FComplexCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    unit_test::check_buffer_destruction<fcomplex>(r3, max);
}

TEST(BasicFunctions, FComplexDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    unit_test::dec_buffer_ref_count<fcomplex>(r3);
}

TEST(BasicFunctions, FComplexChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex c = 1if;
    unit_test::change_one_buffer<fcomplex>(r3, max, c);
}

TEST(BasicFunctions, FComplexMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex c1 = 5.f + 2if;
    fcomplex c2 = 1.f + 2if;
    unit_test::multi_change_buffer<fcomplex>(r3, max, c1, c2);
}

TEST(BasicFunctions, FComplexSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    unit_test::set_buffer_ptr_array<fcomplex>(r3);
    // TODO: buffer array here is 2 times longer
}

TEST(BasicFunctions, FComplexQueueBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex c = 1if;
    unit_test::queue_buffer_from_fill<fcomplex>(r3, max, c);
}

TEST(BasicFunctions, FComplexDecOperate) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max);
    unit_test::dec_operate_queue<fcomplex>(r3);
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

TEST(Concurrency, ThreadFComplexSeesBufferChanges) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex c = 1if;
    unit_test::change_buffer_threaded<fcomplex>(r3, c);
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

TEST(Concurrency, ThreadFComplexMultiBufferChange) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex data[3] = {5.f, 9if, 6.f - 6if};
    unit_test::multi_change_buffer_threaded<fcomplex>(r3, data);
}

TEST(Concurrency, ThreadIntWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::wait_on_fill_threaded<int>(r3, max, 5);
}

TEST(Concurrency, ThreadFloatWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<float>> r3 = 
        std::make_shared<recycle_memory<float>>(shape, max); 
    unit_test::wait_on_fill_threaded<float>(r3, max, 5.0);
}

TEST(Concurrency, ThreadFComplexWaitsForFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex c = 5.f - 5if;
    unit_test::wait_on_fill_threaded<fcomplex>(r3, max, c);
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

TEST(Concurrency, ThreadFComplexWaitsForQueue) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex c = 5.f - 5if;
    unit_test::buffer_from_empty_queue_threaded<fcomplex>(r3, max, c);
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

TEST(Concurrency, ThreadFComplexMultiWaitsForFill) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<fcomplex>> r3 = 
        std::make_shared<recycle_memory<fcomplex>>(shape, max); 
    fcomplex c = 5.f - 5if;
    unit_test::wait_multi_take_from_fill_threaded<fcomplex>(r3, max, c);
}

TEST(Concurrency, ThreadWatchesBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<recycle_memory<int>> r3 = 
        std::make_shared<recycle_memory<int>>(shape, max); 
    unit_test::watcher_check_reference_counts<int>(r3);
}