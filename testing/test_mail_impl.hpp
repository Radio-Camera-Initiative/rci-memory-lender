#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <complex>
#include <mutex>
#include <condition_variable>

#include "../lender.hpp"
#include "gtest/gtest.h"
#include "test_mail_class.hpp"

#define UnexpectedEq(check, value, name) EXPECT_EQ(check, value) \
    << "Unexpected " name "."

template <typename T>
void mail_test::make_mailbox(std::vector<size_t> shape, int max) {
// -> check correct number of free
    mailbox<T> r(shape, max);

    UnexpectedEq(r.private_free_size(), max, "free list size");
    // TODO: add way to pull all keys && check that it's empty?
}

template <typename T>
void mail_test::mailbox_queue_one(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    
    // check that the recycler has something in map
    // check that key exists in map
    EXPECT_TRUE(recycler->contains_key(10));
}

template <typename T>
void mail_test::mailbox_queue_multi_buffer(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_FALSE(recycler->contains_key(7));
    EXPECT_FALSE(recycler->contains_key(45));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));

    buffer_ptr<T> p2 = recycler->fill();
    UnexpectedEq(p2.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(7));

    recycler->queue(7, p2);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    
    buffer_ptr<T> p3 = recycler->fill();
    UnexpectedEq(p3.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(45));

    recycler->queue(45, p3);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
}

// check for same ptr multiple keys
template <typename T>
void mail_test::mailbox_multi_queue_one_buffer(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_FALSE(recycler->contains_key(7));
    EXPECT_FALSE(recycler->contains_key(45));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));

    recycler->queue(7, p);
    UnexpectedEq(p.use_count(), 3, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));

    recycler->queue(45, p);
    UnexpectedEq(p.use_count(), 4, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));

}

// single entry read
template <typename T>
void mail_test::mailbox_read_single_entry_map(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 2, "reference count");
        EXPECT_FALSE(recycler->contains_key(10));
    }

    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
}

// multi buffer entry read
template <typename T>
void mail_test::mailbox_read_multi_entry_map(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));

    buffer_ptr<T> p2 = recycler->fill();
    UnexpectedEq(p2.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(7));

    buffer_ptr<T> p3 = recycler->fill();
    UnexpectedEq(p3.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(45));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    
    recycler->queue(7, p2);
    UnexpectedEq(p2.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));

    recycler->queue(45, p3);
    UnexpectedEq(p3.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 2, "reference count");
        UnexpectedEq(b.use_count(), 2, "reference count");
        EXPECT_FALSE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
}

// multi entry read one buffer
template <typename T>
void mail_test::mailbox_read_multi_entry_one_buffer(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_FALSE(recycler->contains_key(7));
    EXPECT_FALSE(recycler->contains_key(45));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    
    recycler->queue(7, p);
    UnexpectedEq(p.use_count(), 3, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));

    recycler->queue(45, p);
    UnexpectedEq(p.use_count(), 4, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 4, "reference count");
        UnexpectedEq(b.use_count(), 4, "reference count");
        EXPECT_FALSE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    UnexpectedEq(p.use_count(), 3, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
}

// also with more than one read allowed
template <typename T>
void mail_test::mailbox_multi_read_single_entry_map(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 3, "reference count");
        EXPECT_TRUE(recycler->contains_key(10));
    }

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 2, "reference count");
        EXPECT_FALSE(recycler->contains_key(10));
    }

    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
}

// multi buffer entry read
template <typename T>
void mail_test::mailbox_multi_read_multi_entry_map(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));

    buffer_ptr<T> p2 = recycler->fill();
    UnexpectedEq(p2.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(7));

    buffer_ptr<T> p3 = recycler->fill();
    UnexpectedEq(p3.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(45));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    
    recycler->queue(7, p2);
    UnexpectedEq(p2.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));

    recycler->queue(45, p3);
    UnexpectedEq(p3.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 3, "reference count");
        EXPECT_TRUE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 3, "reference count");
        EXPECT_TRUE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 2, "reference count");
        UnexpectedEq(b.use_count(), 2, "reference count");
        EXPECT_FALSE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
}

// multi entry read one buffer
template <typename T>
void mail_test::mailbox_multi_read_multi_entry_one_buffer(
    std::shared_ptr<mailbox<T>> recycler
) {
    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_FALSE(recycler->contains_key(7));
    EXPECT_FALSE(recycler->contains_key(45));

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    
    recycler->queue(7, p);
    UnexpectedEq(p.use_count(), 3, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));

    recycler->queue(45, p);
    UnexpectedEq(p.use_count(), 4, "reference count");
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 5, "reference count");
        EXPECT_TRUE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 5, "reference count");
        EXPECT_TRUE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    {
        buffer_ptr<T> b = recycler->operate(10);
        UnexpectedEq(p.use_count(), 4, "reference count");
        UnexpectedEq(b.use_count(), 4, "reference count");
        EXPECT_FALSE(recycler->contains_key(10));
        EXPECT_TRUE(recycler->contains_key(7));
        EXPECT_TRUE(recycler->contains_key(45));
    }

    UnexpectedEq(p.use_count(), 3, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
}

template <typename T>
void mail_test::fthread_wait_for_mail (
    std::shared_ptr<mailbox<T>> recycler,
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe,
    int key,
    T data
) {
    // try and take. make sure key is not made yet
    EXPECT_FALSE(recycler->contains_key(key));

    // check thread is waiting
    waiting_unsafe = true;
    cv->notify_all();
    auto b = recycler->operate(key);

    // check expected data value
    UnexpectedEq(*b, data, "buffer value");

    // (if the last one check that the key doesnt exist)
    EXPECT_FALSE(recycler->contains_key(key));
    UnexpectedEq(b.use_count(), 2, "reference count");
    waiting_unsafe = false;
    
}

template <typename T>
void mail_test::wait_read_single_entry (
    std::shared_ptr<mailbox<T>> recycler,
    T val
) {
    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::mutex m;

    std::thread check(fthread_wait_for_mail<T>, recycler, std::ref(cv), std::ref(waiting_unsafe), 10, val);

    std::unique_lock<std::mutex> lk(m);
    while(!waiting_unsafe) {
        cv->wait(lk);
    }

    ASSERT_TRUE(waiting_unsafe) <<
        "Thread did not wait for buffer to be available";

    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_FALSE(recycler->contains_key(7));
    EXPECT_FALSE(recycler->contains_key(45));
    
    *p = val;

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");

    check.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";
}

template <typename T>
void mail_test::wait_read_multi_entry (
    std::shared_ptr<mailbox<T>> recycler,
    T val
) {
    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::mutex m;

    std::thread check(fthread_wait_for_mail<T>, recycler, std::ref(cv), std::ref(waiting_unsafe), 10, val);

    std::unique_lock<std::mutex> lk(m);
    while(!waiting_unsafe) {
        cv->wait(lk);
    }

    ASSERT_TRUE(waiting_unsafe) <<
        "Thread did not wait for buffer to be available";

    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(10)); 
    // TODO: actually this^ is not guarenteed, because we "wait" in the other thread and that should make the key
    EXPECT_FALSE(recycler->contains_key(7));
    EXPECT_FALSE(recycler->contains_key(45));
    buffer_ptr<T> f = recycler->fill();
    UnexpectedEq(f.use_count(), 1, "reference count");
    recycler->queue(7, f);
    recycler->queue(45, f);
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
    UnexpectedEq(f.use_count(), 3, "reference count");
    
    *p = val;

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");

    check.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";
}

template <typename T>
void mail_test::fthread_wait_for_mail_multi (
    std::shared_ptr<mailbox<T>> recycler,
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe,
    int key,
    T data
) {
    // check thread is waiting
    waiting_unsafe = true;
    cv->notify_all();
    auto b = recycler->operate(key);

    // check expected data value
    UnexpectedEq(*b, data, "buffer value");

    waiting_unsafe = false;
    
}


template <typename T>
void mail_test::multi_wait_read_single_entry (
    std::shared_ptr<mailbox<T>> recycler,
    T val
) {
    EXPECT_FALSE(recycler->contains_key(10));

    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::mutex m;

    std::thread read1(fthread_wait_for_mail_multi<T>, recycler, std::ref(cv), std::ref(waiting_unsafe), 10, val);
    std::unique_lock<std::mutex> lk(m);
    while(!waiting_unsafe) {
        cv->wait(lk);
    }

    ASSERT_TRUE(waiting_unsafe) <<
        "Thread did not wait for buffer to be available";

    bool waiting_unsafe2 = false;
    std::shared_ptr<std::condition_variable> cv2 = 
        std::make_shared<std::condition_variable>();
    std::mutex m2;

    std::thread read2(fthread_wait_for_mail_multi<T>, recycler, std::ref(cv2), std::ref(waiting_unsafe2), 10, val);

    std::unique_lock<std::mutex> lk2(m2);
    while(!waiting_unsafe2) {
        cv2->wait(lk2);
    }

    ASSERT_TRUE(waiting_unsafe2) <<
        "Thread did not wait for buffer to be available";


    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    *p = val;

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");

    read1.join();
    read2.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";

    EXPECT_FALSE(waiting_unsafe2) <<
        "Thread did not end and join correctly";
    
    EXPECT_FALSE(recycler->contains_key(10));
}

template <typename T>
void mail_test::multi_wait_read_multi_entry (
    std::shared_ptr<mailbox<T>> recycler,
    T val
) {
    EXPECT_FALSE(recycler->contains_key(10));

    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::mutex m;
    bool test = true;
    std::thread read1(fthread_wait_for_mail_multi<T>, recycler, std::ref(cv), std::ref(waiting_unsafe), 10, val);

    std::unique_lock<std::mutex> lk(m);
    while(!waiting_unsafe) {
        cv->wait(lk);
    }

    ASSERT_TRUE(waiting_unsafe) <<
        "Thread did not wait for buffer to be available";

    bool waiting_unsafe2 = false;
    std::shared_ptr<std::condition_variable> cv2 = 
        std::make_shared<std::condition_variable>();
    std::mutex m2;

    std::thread read2(fthread_wait_for_mail_multi<T>, recycler, std::ref(cv2), std::ref(waiting_unsafe2), 10, val);

    std::unique_lock<std::mutex> lk2(m2);
    while(!waiting_unsafe2) {
        cv2->wait(lk2);
    }

    ASSERT_TRUE(waiting_unsafe2) <<
        "Thread did not wait for buffer to be available";

    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(7));
    EXPECT_FALSE(recycler->contains_key(45));
    buffer_ptr<T> f = recycler->fill();
    UnexpectedEq(f.use_count(), 1, "reference count");
    recycler->queue(7, f);
    recycler->queue(45, f);
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
    UnexpectedEq(f.use_count(), 3, "reference count");
    
    *p = val;

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");

    read1.join();
    read2.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";

    EXPECT_FALSE(waiting_unsafe2) <<
        "Thread did not end and join correctly";
    
    EXPECT_FALSE(recycler->contains_key(10));
}

template <typename T>
void mail_test::multi_wait_read_diff_entry (
    std::shared_ptr<mailbox<T>> recycler,
    T val
) {
    EXPECT_FALSE(recycler->contains_key(10));
    EXPECT_FALSE(recycler->contains_key(7));

    bool waiting_unsafe = false;
    std::shared_ptr<std::condition_variable> cv = 
        std::make_shared<std::condition_variable>();
    std::mutex m;
    bool test = true;
    std::thread read1(fthread_wait_for_mail<T>, recycler, std::ref(cv), std::ref(waiting_unsafe), 10, val);

    std::unique_lock<std::mutex> lk(m);
    while(!waiting_unsafe) {
        cv->wait(lk);
    }

    ASSERT_TRUE(waiting_unsafe) <<
        "Thread did not wait for buffer to be available";

    bool waiting_unsafe2 = false;
    std::shared_ptr<std::condition_variable> cv2 = 
        std::make_shared<std::condition_variable>();
    std::mutex m2;

    std::thread read2(fthread_wait_for_mail<T>, recycler, std::ref(cv2), std::ref(waiting_unsafe2), 7, val);

    std::unique_lock<std::mutex> lk2(m2);
    while(!waiting_unsafe2) {
        cv2->wait(lk2);
    }

    ASSERT_TRUE(waiting_unsafe2) <<
        "Thread did not wait for buffer to be available";

    buffer_ptr<T> p = recycler->fill();
    UnexpectedEq(p.use_count(), 1, "reference count");
    EXPECT_FALSE(recycler->contains_key(45));
    buffer_ptr<T> f = recycler->fill();
    UnexpectedEq(f.use_count(), 1, "reference count");
    buffer_ptr<T> q = recycler->fill();
    UnexpectedEq(q.use_count(), 1, "reference count");
    recycler->queue(45, q);
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(10));
    EXPECT_TRUE(recycler->contains_key(45));
    
    *p = val;
    *f = val;

    recycler->queue(10, p);
    UnexpectedEq(p.use_count(), 2, "reference count");

    recycler->queue(7, f);
    UnexpectedEq(f.use_count(), 2, "reference count");

    read1.join();
    read2.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";

    EXPECT_FALSE(waiting_unsafe2) <<
        "Thread did not end and join correctly";
    
    EXPECT_FALSE(recycler->contains_key(10));
}

// ALSO add asynchronous (serial reads)