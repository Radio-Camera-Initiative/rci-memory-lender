#include "gtest/gtest.h"
#include "test_mail_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

template <typename T>
class MailboxTest : public testing::Test {
    protected:
        void SetUp() override {
            max = 3;
            shape = std::vector<size_t>(1, 1);
            mailbox_1 = std::make_shared<mailbox<T>>(shape, max);
            mailbox_2 = std::make_shared<mailbox<T>>(shape, max, 2);
            mailbox_3 = std::make_shared<mailbox<T>>(shape, max, 3);
        }

        // void TearDown() override {}

    public:
        int max;
        std::vector<size_t> shape;
        std::shared_ptr<mailbox<T>> mailbox_1 = nullptr;
        std::shared_ptr<mailbox<T>> mailbox_2 = nullptr;
        std::shared_ptr<mailbox<T>> mailbox_3 = nullptr;
};

using Primitives = ::testing::Types<int, float, fcomplex>;
TYPED_TEST_SUITE(MailboxTest, Primitives);

TYPED_TEST(MailboxTest, MakeMailbox) {
    mail_test::make_mailbox<TypeParam>(this->shape, this->max);
}

TYPED_TEST(MailboxTest, QueueOneBuffer) {
    mail_test::mailbox_queue_one<TypeParam>(this->mailbox_1);
}

TYPED_TEST(MailboxTest, QueueMultipleBuffer) { 
    mail_test::mailbox_queue_multi_buffer<TypeParam>(this->mailbox_1);
}

TYPED_TEST(MailboxTest, MultiQueueOneBuffer) {
    mail_test::mailbox_multi_queue_one_buffer<TypeParam>(this->mailbox_1);
}

TYPED_TEST(MailboxTest, SingleEntrySingleRead) {
    mail_test::mailbox_read_single_entry_map<TypeParam>(this->mailbox_1);
}

TYPED_TEST(MailboxTest, MultiEntrySingleRead) {
    mail_test::mailbox_read_multi_entry_map<TypeParam>(this->mailbox_1);
}

TYPED_TEST(MailboxTest, MultiEntrySingleBufferRead) {
    mail_test::mailbox_read_multi_entry_one_buffer<TypeParam>(this->mailbox_1);
}

TYPED_TEST(MailboxTest, SingleEntryMultiRead) {
    mail_test::mailbox_multi_read_single_entry_map<TypeParam>(this->mailbox_2);
}

TYPED_TEST(MailboxTest, MultiEntryMultiRead) {
    mail_test::mailbox_multi_read_multi_entry_map<TypeParam>(this->mailbox_3);
}

TYPED_TEST(MailboxTest, MultiEntrySingleBufferMultiRead) {
    mail_test::mailbox_multi_read_multi_entry_one_buffer<TypeParam>(this->mailbox_3);
}

TEST(MailboxThreaded, IntWaitSingleEntry) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::wait_read_single_entry<int>(r3, 9);
}

TEST(MailboxThreaded, IntWaitMultiEntry) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::wait_read_multi_entry<int>(r3, 9);
}

TEST(MailboxThreaded, IntMultiWaitSingleEntry) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max, 2); 
    mail_test::multi_wait_read_single_entry<int>(r3, 9);
}

TEST(MailboxThreaded, IntMultiWaitMultiEntry) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max, 2); 
    mail_test::multi_wait_read_multi_entry<int>(r3, 9);
}

TEST(MailboxThreaded, IntMultiWaitDiffEntry) {
    int max = 3;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::multi_wait_read_diff_entry<int>(r3, 9);
}