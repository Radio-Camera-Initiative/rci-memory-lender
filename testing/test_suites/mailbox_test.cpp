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

TEST(MailboxDuplicates, IntDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<int>> r3 = 
        std::make_shared<mailbox<int>>(shape, max); 
    mail_test::dec_buffer_ref_count<int>(r3);
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

TEST(MailboxDuplicates, FloatDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<float>> r3 = 
        std::make_shared<mailbox<float>>(shape, max); 
    mail_test::dec_buffer_ref_count<float>(r3);
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

TEST(MailboxDuplicates, FComplexDecRefCount) {
    int max = 1;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<mailbox<fcomplex>> r3 = 
        std::make_shared<mailbox<fcomplex>>(shape, max); 
    mail_test::dec_buffer_ref_count<fcomplex>(r3);
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