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
            mailbox_0 = std::make_shared<mailbox<T>>(shape, max, 0);
            mailbox_1 = std::make_shared<mailbox<T>>(shape, max);
            mailbox_2 = std::make_shared<mailbox<T>>(shape, max, 2);
            mailbox_3 = std::make_shared<mailbox<T>>(shape, max, 3);
        }

        // void TearDown() override {}

    public:
        int max;
        std::vector<size_t> shape;
        std::shared_ptr<mailbox<T>> mailbox_0 = nullptr;
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

TYPED_TEST(MailboxTest, WaitSingleEntry) {
    mail_test::wait_read_single_entry<TypeParam>(this->mailbox_1, 9);
}

TYPED_TEST(MailboxTest, WaitMultiEntry) {
    mail_test::wait_read_multi_entry<TypeParam>(this->mailbox_1, 9);
}

TYPED_TEST(MailboxTest, MultiWaitSingleEntry) {
    mail_test::multi_wait_read_single_entry<TypeParam>(this->mailbox_2, 9);
}

TYPED_TEST(MailboxTest, MultiWaitMultiEntry) {
    mail_test::multi_wait_read_multi_entry<TypeParam>(this->mailbox_2, 9);
}

TYPED_TEST(MailboxTest, MultiWaitDiffEntry) {
    mail_test::multi_wait_read_diff_entry<TypeParam>(this->mailbox_1, 9);
}

TYPED_TEST(MailboxTest, ExtraSingleEntry) {
    mail_test::extra_read_single_entry<TypeParam>(this->mailbox_0, 9);
}

TYPED_TEST(MailboxTest, ExtraWaitSingleEntry) {
    mail_test::extra_wait_read_single_entry<TypeParam>(this->mailbox_1, 9);
}