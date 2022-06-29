#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

TEST(BasicFunctions, NullBufferPtr) {
    unit_test::null_buffer_ptr<int>();
    unit_test::null_buffer_ptr<float>();
    unit_test::null_buffer_ptr<fcomplex>();
}

TEST(BasicFunctions, BufferPtrNullFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::buffer_ptr_null_fill<int>(r3, shape, max);
}

TEST(BasicFunctions, BufferPtrFillNull) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::buffer_ptr_fill_null<int>(r3, shape, max);
}

TEST(BasicFunctions, IntMakeRecycleMemory) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    unit_test::make_recycle_memory<int>(shape, max);
}

TEST(BasicFunctions, IntTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::take_one_buffer_from_fill<int>(r3, shape, max);
}

TEST(BasicFunctions, IntCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    unit_test::check_buffer_destruction<int>(r3, max);
}

TEST(BasicFunctions, IntDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::dec_buffer_ref_count<int>(r3);
}

TEST(BasicFunctions, IntChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::change_one_buffer<int>(r3, max, 5);
}

TEST(BasicFunctions, IntMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::multi_change_buffer<int>(r3, max, 5, 2);
}

TEST(BasicFunctions, IntSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::set_buffer_ptr_array<int>(r3);
}

TEST(BasicFunctions, IntQueueBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max); 
    unit_test::queue_buffer_from_fill<int>(r3, max, 5);
}

TEST(BasicFunctions, IntDecOperate) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> r3 = 
        std::make_shared<library<int>>(shape, max);
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
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::take_one_buffer_from_fill<float>(r3, shape, max);
}

TEST(BasicFunctions, FloatCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    unit_test::check_buffer_destruction<float>(r3, max);
}

TEST(BasicFunctions, FloatDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::dec_buffer_ref_count<float>(r3);
}

TEST(BasicFunctions, FloatChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::change_one_buffer<float>(r3, max, 5.0);
}

TEST(BasicFunctions, FloatMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::multi_change_buffer<float>(r3, max, 5.0, 2.0);
}

TEST(BasicFunctions, FloatSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::set_buffer_ptr_array<float>(r3);
}

TEST(BasicFunctions, FloatQueueBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max); 
    unit_test::queue_buffer_from_fill<float>(r3, max, 5.0);
}

TEST(BasicFunctions, FloatDecOperate) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<float>> r3 = 
        std::make_shared<library<float>>(shape, max);
    unit_test::dec_operate_queue<float>(r3);
}

TEST(BasicFunctions, FComplexMakeRecycleMemory) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    unit_test::make_recycle_memory<fcomplex>(shape, max);
}

TEST(BasicFunctions, FComplexTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    unit_test::take_one_buffer_from_fill<fcomplex>(r3, shape, max);
}

TEST(BasicFunctions, FComplexCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    unit_test::check_buffer_destruction<fcomplex>(r3, max);
}

TEST(BasicFunctions, FComplexDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    unit_test::dec_buffer_ref_count<fcomplex>(r3);
}

TEST(BasicFunctions, FComplexChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex c = 1if;
    unit_test::change_one_buffer<fcomplex>(r3, max, c);
}

TEST(BasicFunctions, FComplexMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex c1 = 5.f + 2if;
    fcomplex c2 = 1.f + 2if;
    unit_test::multi_change_buffer<fcomplex>(r3, max, c1, c2);
}

TEST(BasicFunctions, FComplexSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    unit_test::set_buffer_ptr_array<fcomplex>(r3);
    // TODO: buffer array here is 2 times longer
}

TEST(BasicFunctions, FComplexQueueBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max); 
    fcomplex c = 1if;
    unit_test::queue_buffer_from_fill<fcomplex>(r3, max, c);
}

TEST(BasicFunctions, FComplexDecOperate) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<fcomplex>> r3 = 
        std::make_shared<library<fcomplex>>(shape, max);
    unit_test::dec_operate_queue<fcomplex>(r3);
}
