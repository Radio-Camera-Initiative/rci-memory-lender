#include "gtest/gtest.h"
#include "test_class.hpp"
#include "test_mail_class.hpp"
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

TYPED_TEST(PipelineTest, MailThreadsSameKey) {
    int buffers = 1;
    int threads = 20;
    std::shared_ptr<mailbox<TypeParam>> r3 = 
        std::make_shared<mailbox<TypeParam>>(this->shape, buffers, threads); 

    mail_test::wait_m_threads_same_key<TypeParam>(r3, threads, 9);
}

TYPED_TEST(PipelineTest, MailThreadsDiffKey) {
    int buffers = 1;
    int threads = 30;
    std::vector<int> fills = std::vector<int>(threads);
    std::iota(std::begin(fills), std::end(fills), 0);
    std::shared_ptr<mailbox<TypeParam>> r3 = 
        std::make_shared<mailbox<TypeParam>>(this->shape, buffers); 

    mail_test::wait_m_threads_diff_key<TypeParam>(r3, fills, threads);
}