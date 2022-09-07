#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

template <typename T>
class LibraryTest : public testing::Test {
    protected:
        void SetUp() override {
            max = 2;
            shape = std::vector<size_t>(1, 1);
            recycler = std::make_shared<library<T>>(shape, max);
        }

        // void TearDown() override {}

    public:
        int max;
        std::vector<size_t> shape;
        std::shared_ptr<library<T>> recycler = nullptr;
};

using Primitives = ::testing::Types<int, float, fcomplex>;
TYPED_TEST_SUITE(LibraryTest, Primitives);

TYPED_TEST(LibraryTest, MakeRecycleMemory) {
    unit_test::make_recycle_memory<TypeParam>(this->shape, this->max);
}

TYPED_TEST(LibraryTest, QueueBuffer) {
    unit_test::queue_buffer_from_fill<TypeParam>(this->recycler, this->max, 5);
}

TYPED_TEST(LibraryTest, DecOperate) {
    unit_test::dec_operate_queue<TypeParam>(this->recycler, this->max);
}

TYPED_TEST(LibraryTest, ThreadWaitsForQueue) {
    unit_test::buffer_from_empty_queue_threaded<TypeParam>(this->recycler, this->max, 5);
}