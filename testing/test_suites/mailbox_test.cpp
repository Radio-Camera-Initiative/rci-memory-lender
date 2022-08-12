#include "gtest/gtest.h"
#include "test_mail_class.hpp"
#include "../lender.hpp"
#include <complex>
#include <numeric>

typedef std::complex<float> fcomplex;
using namespace std::complex_literals;

TEST(MailboxDuplicates, IntMakeMailbox) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    mail_test::make_mailbox<int>(shape, max);
}

TEST(MailboxDuplicates, IntQueueOneBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::mailbox_queue_one<int>(r3);
}

TEST(MailboxDuplicates, IntQueueMultipleBuffer) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::mailbox_queue_multi_buffer<int>(r3);
}

TEST(MailboxDuplicates, IntMultiQueueOneBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::mailbox_multi_queue_one_buffer<int>(r3);
}

TEST(MailboxDuplicates, IntSingleEntrySingleRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::mailbox_read_single_entry_map<int>(r3);
}

TEST(MailboxDuplicates, IntMultiEntrySingleRead) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::mailbox_read_multi_entry_map<int>(r3);
}

TEST(MailboxDuplicates, IntMultiEntrySingleBufferRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::mailbox_read_multi_entry_one_buffer<int>(r3);
}

TEST(MailboxDuplicates, IntSingleEntryMultiRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max, 2); 
    mail_test::mailbox_multi_read_single_entry_map<int>(r3);
}

TEST(MailboxDuplicates, IntMultiEntryMultiRead) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max, 3); 
    mail_test::mailbox_multi_read_multi_entry_map<int>(r3);
}

TEST(MailboxDuplicates, IntMultiEntrySingleBufferMultiRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max, 3); 
    mail_test::mailbox_multi_read_multi_entry_one_buffer<int>(r3);
}

TEST(MailboxDuplicates, FloatMakeMailbox) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    mail_test::make_mailbox<float>(shape, max);
}

TEST(MailboxDuplicates, FloatQueueOneBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::mailbox_queue_one<float>(r3);
}

TEST(MailboxDuplicates, FloatQueueMultipleBuffer) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::mailbox_queue_multi_buffer<float>(r3);
}

TEST(MailboxDuplicates, FloatMultiQueueOneBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::mailbox_multi_queue_one_buffer<float>(r3);
}

TEST(MailboxDuplicates, FloatSingleEntrySingleRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::mailbox_read_single_entry_map<float>(r3);
}

TEST(MailboxDuplicates, FloatMultiEntrySingleRead) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::mailbox_read_multi_entry_map<float>(r3);
}

TEST(MailboxDuplicates, FloatMultiEntrySingleBufferRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::mailbox_read_multi_entry_one_buffer<float>(r3);
}

TEST(MailboxDuplicates, FloatSingleEntryMultiRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max, 2); 
    mail_test::mailbox_multi_read_single_entry_map<float>(r3);
}

TEST(MailboxDuplicates, FloatMultiEntryMultiRead) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max, 3); 
    mail_test::mailbox_multi_read_multi_entry_map<float>(r3);
}

TEST(MailboxDuplicates, FloatMultiEntrySingleBufferMultiRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max, 3); 
    mail_test::mailbox_multi_read_multi_entry_one_buffer<float>(r3);
}


TEST(MailboxDuplicates, FComplexMakeMailbox) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    mail_test::make_mailbox<fcomplex>(shape, max);
}

TEST(MailboxDuplicates, FComplexQueueOneBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::mailbox_queue_one<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexQueueMultipleBuffer) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::mailbox_queue_multi_buffer<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexMultiQueueOneBuffer) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::mailbox_multi_queue_one_buffer<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexSingleEntrySingleRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::mailbox_read_single_entry_map<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexMultiEntrySingleRead) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::mailbox_read_multi_entry_map<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexMultiEntrySingleBufferRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::mailbox_read_multi_entry_one_buffer<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexSingleEntryMultiRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max, 2); 
    mail_test::mailbox_multi_read_single_entry_map<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexMultiEntryMultiRead) {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max, 3); 
    mail_test::mailbox_multi_read_multi_entry_map<fcomplex>(r3);
}

TEST(MailboxDuplicates, FComplexMultiEntrySingleBufferMultiRead) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max, 3); 
    mail_test::mailbox_multi_read_multi_entry_one_buffer<fcomplex>(r3);
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