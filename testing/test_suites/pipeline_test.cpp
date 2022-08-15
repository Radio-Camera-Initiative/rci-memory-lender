#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

template <typename T>
class PipelineTest : public testing::Test {
    protected:
        void SetUp() override {
            std::iota(std::begin(filling), std::end(filling), 0);
            shape = std::vector<size_t>(1, 1);
        }

        // void TearDown() override {}

    public:
        std::vector<size_t> shape;
        std::vector<T> filling = std::vector<T>(30);
};

using Primitives = ::testing::Types<int, float, fcomplex>;
TYPED_TEST_SUITE(PipelineTest, Primitives);

TYPED_TEST(PipelineTest, EqualThreadsBuffers) {
    int buffers = 5;
    int threads = 5;
    std::shared_ptr<library<TypeParam>> r3 = 
        std::make_shared<library<TypeParam>>(this->shape, buffers);

    unit_test::run_m_threads_n_buffers<TypeParam>(r3, this->filling, threads);
}

TYPED_TEST(PipelineTest, MoreThreads) {
    int buffers = 5;
    int threads = 10;
    std::shared_ptr<library<TypeParam>> r3 = 
        std::make_shared<library<TypeParam>>(this->shape, buffers); 

    unit_test::run_m_threads_n_buffers<TypeParam>(r3, this->filling, threads);
}

TYPED_TEST(PipelineTest, MoreBuffers) {
    int buffers = 10;
    int threads = 5;
    std::shared_ptr<library<TypeParam>> r3 = 
        std::make_shared<library<TypeParam>>(this->shape, buffers); 
        
    unit_test::run_m_threads_n_buffers<TypeParam>(r3, this->filling, threads);
}