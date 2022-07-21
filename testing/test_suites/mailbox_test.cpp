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

TEST(MailboxDuplicates, IntTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::take_one_buffer_from_fill<int>(r3, shape, max);
}

TEST(MailboxDuplicates, IntCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    mail_test::check_buffer_destruction<int>(r3, max);
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

TEST(MailboxDuplicates, IntChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::change_one_buffer<int>(r3, max, 5);
}

TEST(MailboxDuplicates, IntMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::multi_change_buffer<int>(r3, max, 5, 2);
}

TEST(MailboxDuplicates, IntSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::set_buffer_ptr_array<int>(r3);
}


TEST(MailboxDuplicates, FloatMakeMailbox) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    mail_test::make_mailbox<float>(shape, max);
}

TEST(MailboxDuplicates, FloatTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::take_one_buffer_from_fill<float>(r3, shape, max);
}

TEST(MailboxDuplicates, FloatCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    mail_test::check_buffer_destruction<float>(r3, max);
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

TEST(MailboxDuplicates, FloatChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::change_one_buffer<float>(r3, max, 5.0);
}

TEST(MailboxDuplicates, FloatMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::multi_change_buffer<float>(r3, max, 5.0, 2.0);
}

TEST(MailboxDuplicates, FloatSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::set_buffer_ptr_array<float>(r3);
}

TEST(MailboxDuplicates, FComplexMakeMailbox) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    mail_test::make_mailbox<fcomplex>(shape, max);
}

TEST(MailboxDuplicates, FComplexTakeWithFill) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::take_one_buffer_from_fill<fcomplex>(r3, shape, max);
}

TEST(MailboxDuplicates, FComplexCheckBufferDestruction) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    // unit_test::take_one_buffer_from_fill(r3, shape, max);
    mail_test::check_buffer_destruction<fcomplex>(r3, max);
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

TEST(MailboxDuplicates, FComplexChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    fcomplex c = 1if;
    mail_test::change_one_buffer<fcomplex>(r3, max, c);
}

TEST(MailboxDuplicates, FComplexMultiChangeBuffers) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    fcomplex c1 = 5.f + 2if;
    fcomplex c2 = 1.f + 2if;
    mail_test::multi_change_buffer<fcomplex>(r3, max, c1, c2);
}

TEST(MailboxDuplicates, FComplexSetBufferArray) {
    int max = 2;
    std::vector<size_t> shape = std::vector<size_t>(2, 2);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::set_buffer_ptr_array<fcomplex>(r3);
    // TODO: buffer array here is 2 times longer
}