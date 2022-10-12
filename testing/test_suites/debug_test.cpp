#include "gtest/gtest.h"
#include "test_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

template <typename T>
class DebugTest : public testing::Test {
    protected:
        void SetUp() override {
            zero = 0;
            max = 1;
            shape = std::vector<size_t>(1, 1);
            recycler = std::make_shared<library<T>>(shape, max);
            alt_recycler = std::make_shared<library<T>>(shape, max);
            mail = std::make_shared<mailbox<T>>(shape, max);
        }

        // void TearDown() override {}

    public:
        int max;
        std::vector<size_t> shape;
        std::vector<size_t> array_shape;
        std::shared_ptr<library<T>> recycler = nullptr;
        std::shared_ptr<library<T>> alt_recycler = nullptr;
        std::shared_ptr<mailbox<T>> mail = nullptr;
        T zero;
};

using Primitives = ::testing::Types<int, float, fcomplex>;
TYPED_TEST_SUITE(DebugTest, Primitives);

#ifndef NODEBUG

TYPED_TEST(DebugTest, LibraryMemoryCheckZero) {
    {
        auto buffer = this->recycler->fill();
        EXPECT_EQ(*buffer, this->zero);
        *buffer = 1;
    }
    auto buffer = this->recycler->fill();
    EXPECT_EQ(*buffer, this->zero);
}

TYPED_TEST(DebugTest, LibraryMemoryCheckF0) {
    TypeParam* raw_ptr;
    {
        auto buffer = this->recycler->fill();
        ASSERT_EQ(*buffer, this->zero);
        raw_ptr = buffer.get();
    }

    TypeParam* f = new TypeParam();
    memset(reinterpret_cast<void*>(f), 0xf0, sizeof(TypeParam));
    EXPECT_EQ(*f, *raw_ptr);
    delete f;
}

TYPED_TEST(DebugTest, LibraryMemoryCheckUseAfterFreeAssert) {
    TypeParam* raw_ptr;
    {
        auto buffer = this->recycler->fill();
        ASSERT_EQ(*buffer, this->zero);
        raw_ptr = buffer.get();
    }

    *raw_ptr = 1;

    EXPECT_DEATH(this->recycler->fill(), "Assertion .* failed.");
}

TYPED_TEST(DebugTest, LibraryQueueInvalidPointerDiffRecycler) {
    auto p = this->recycler->fill();
    EXPECT_DEATH(this->alt_recycler->queue(p), ".* Assertion .* failed.");
}



TYPED_TEST(DebugTest, MailboxMemoryCheckZero) {
    {
        auto buffer = this->mail->fill();
        EXPECT_EQ(*buffer, this->zero);
        *buffer = 1;
    }
    auto buffer = this->mail->fill();
    EXPECT_EQ(*buffer, this->zero);
}

TYPED_TEST(DebugTest, MailboxMemoryCheckF0) {
    TypeParam* raw_ptr;
    {
        auto buffer = this->mail->fill();
        ASSERT_EQ(*buffer, this->zero);
        raw_ptr = buffer.get();
    }

    TypeParam* f = new TypeParam();
    memset(reinterpret_cast<void*>(f), 0xf0, sizeof(TypeParam));
    EXPECT_EQ(*f, *raw_ptr);
    delete f;
}

TYPED_TEST(DebugTest, MailboxMemoryCheckUseAfterFreeAssert) {
    TypeParam* raw_ptr;
    {
        auto buffer = this->mail->fill();
        ASSERT_EQ(*buffer, this->zero);
        raw_ptr = buffer.get();
    }

    *raw_ptr = 1;

    EXPECT_DEATH(this->mail->fill(), "Assertion .* failed.");
}

TYPED_TEST(DebugTest, MailboxQueueInvalidPointerDiffRecycler) {
    auto p = this->recycler->fill();
    EXPECT_DEATH(this->mail->queue(0, p), ".* Assertion .* failed.");
}

#endif