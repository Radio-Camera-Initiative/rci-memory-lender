#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

template <typename T>
class BufferRecyclerTest : public testing::Test {
    protected:
        void SetUp() override {
            max = 2;
            shape = std::vector<size_t>(1, 1);
            array_shape = std::vector<size_t>(2, 2);
            recycler = std::make_shared<library<T>>(shape, max);
            array_recycler = std::make_shared<library<T>>(array_shape, max);
        }

        // void TearDown() override {}

    public:
        int max;
        std::vector<size_t> shape;
        std::vector<size_t> array_shape;
        std::shared_ptr<library<T>> recycler = nullptr;
        std::shared_ptr<library<T>> array_recycler = nullptr;
};

using Primitives = ::testing::Types<int, float, fcomplex>;
TYPED_TEST_SUITE(BufferRecyclerTest, Primitives);

TYPED_TEST(BufferRecyclerTest, NullBufferPtr) {
    unit_test::null_buffer_ptr<TypeParam>();
}

TYPED_TEST(BufferRecyclerTest, BufferPtrNullFill) {
    unit_test::buffer_ptr_null_fill<TypeParam>(this->recycler, this->shape, this->max);
}

TYPED_TEST(BufferRecyclerTest, BufferPtrFillNull) {
    unit_test::buffer_ptr_fill_null<TypeParam>(this->recycler, this->shape, this->max);
}

TYPED_TEST(BufferRecyclerTest, TakeWithFill) {
    unit_test::take_one_buffer_from_fill<TypeParam>(this->recycler, this->shape, this->max);
}

TYPED_TEST(BufferRecyclerTest, IntCheckBufferDestruction) {
    unit_test::check_buffer_destruction<TypeParam>(this->recycler, this->max);
}

TYPED_TEST(BufferRecyclerTest, DecRefCount) {
    unit_test::dec_buffer_ref_count<TypeParam>(this->recycler);
}

TYPED_TEST(BufferRecyclerTest, ChangeBuffers) {
    unit_test::change_one_buffer<TypeParam>(this->recycler, this->max, 5);
}

TYPED_TEST(BufferRecyclerTest, MultiChangeBuffers) {
    unit_test::multi_change_buffer<TypeParam>(this->recycler, this->max, 5, 2);
}

TYPED_TEST(BufferRecyclerTest, SetBufferArray) { 
    unit_test::set_buffer_ptr_array<TypeParam>(this->array_recycler);
}

TYPED_TEST(BufferRecyclerTest, ThreadSeesBufferChanges) {
    unit_test::change_buffer_threaded<TypeParam>(this->recycler, 5);
}

TYPED_TEST(BufferRecyclerTest, ThreadMultiBufferChange) { 
    TypeParam data[3] = {5, 9, 12};
    unit_test::multi_change_buffer_threaded<TypeParam>(this->recycler, data);
}

TYPED_TEST(BufferRecyclerTest, ThreadWaitsForFill) {
    unit_test::wait_on_fill_threaded<TypeParam>(this->recycler, this->max, 5);
}

TYPED_TEST(BufferRecyclerTest, ThreadMultiWaitsForFill) {
    unit_test::wait_multi_take_from_fill_threaded<TypeParam>(this->recycler, this->max,5);
}

TYPED_TEST(BufferRecyclerTest, ThreadWatchesBuffer) {
    unit_test::watcher_check_reference_counts<TypeParam>(this->recycler);
}