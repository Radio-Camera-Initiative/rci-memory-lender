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
}

// exercise reference counts when taking buffer from fill queue

template <typename T>
void mail_test::take_one_buffer_from_fill(
    std::shared_ptr<mailbox<T>> recycler,
    std::vector<size_t> shape,
    int max
) {
    // -> check shape, number of free (max - 1), num shared_ptr refs
    buffer_ptr<T> p = recycler->fill();

    for (int i = 0; i < shape.size(); i++) {
        UnexpectedEq(p->shape[i], shape[i], "shape");
    }

    UnexpectedEq(recycler->private_free_size(),  max - 1, "number of free buffers");

    UnexpectedEq(p.use_count(), 1, "reference count");

}

// exercise releasing the shared pointer for a buffer and returning memory to
// the recycler

template <typename T>
void mail_test::check_buffer_destruction(
    std::shared_ptr<mailbox<T>> recycler,
    int max
) {
    // -> check number of free is max
    {
        buffer_ptr<T> p = recycler->fill();

        UnexpectedEq(recycler->private_free_size(),  max - 1, "number of free buffers");

        UnexpectedEq(p.use_count(), 1, "reference count");
    }

    // here we assume that destruction of the buffer was done automatically
    // when out of scope

    UnexpectedEq(recycler->private_free_size(),  max, "number of free buffers");

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
// all that but waiting now

// exercise buffers setting data separately from each other

template <typename T>
void mail_test::change_one_buffer(
    std::shared_ptr<mailbox<T>> recycler,
    int max,
    T data
) {
    // -> check new data cannot be accessed from another buffer

    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    *b1 = data;
    *b2 = data;

    UnexpectedEq(*b1, data, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

    *b1 = data + data;

    UnexpectedEq(*b1, data+data, "buffer_value");
    UnexpectedEq(*b2, data, "buffer value");

}

// exercise buffers changing values multiple times and it does not interfere 
// with other buffers

template <typename T>
void mail_test::multi_change_buffer(
    std::shared_ptr<mailbox<T>> recycler,
    int max,
    T data,
    T diff
) {
    // -> check new data cannot be accessed from another buffer

    auto b1 = recycler->fill();
    auto b2 = recycler->fill();

    *b1 = data;
    *b2 = data;

    UnexpectedEq(*b1, data, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

    *b1 = data + diff;

    UnexpectedEq(*b1, data+diff, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

    *b2 = data - diff;
    
    UnexpectedEq(*b1, data+diff, "buffer value");
    UnexpectedEq(*b2, data-diff, "buffer value");

    *b2 = data;

    UnexpectedEq(*b1, data+diff, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");
    
    *b1 = data;

    UnexpectedEq(*b1, data, "buffer value");
    UnexpectedEq(*b2, data, "buffer value");

}

// exercise setting a buffer (two different ways)

template <typename T>
void mail_test::set_buffer_ptr_array (
    std::shared_ptr<mailbox<T>> recycler
) {
    auto buffer = recycler->fill();
    auto shape = buffer->shape;

    size_t size = 1;
    for (auto iter = shape.begin(); iter != shape.end(); iter++) {
        size *= *iter;
    }

    // fill each index with its number
    T i = 0;
    T one = 1;
    for (size_t idx = 0; idx < size; idx++, i+one) {
        buffer[idx] = i;
        UnexpectedEq(buffer[idx], i, "array value");
    }
}

template <typename T>
void mail_test::wait_for_mail (
    std::shared_ptr<mailbox<T>> recycler,
    std::shared_ptr<std::condition_variable> cv,
    bool &waiting_unsafe,
    int key,
    T data
) {
    // try and take. make sure key is not made yet
    EXPECT_FALSE(recycler->contains_key(10));

    // check thread is waiting
    waiting_unsafe = true;
    cv->notify_all();
    auto b = recycler->operate(key);

    // check expected data value
    UnexpectedEq(*b, data, "buffer value");

    // TODO: check use_count 1 or 2 by whether mailbox is still full
    // (if the last one check that the key doesnt exist)
    EXPECT_FALSE(recycler->contains_key(10));
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

    std::thread check(wait_for_mail<T>, recycler, std::ref(cv), std::ref(waiting_unsafe), 10, val);

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

    recycler->queue(10, p); // no time to check if it contains
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

    std::thread check(wait_for_mail<T>, recycler, std::ref(cv), std::ref(waiting_unsafe), 10, val);

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
    buffer_ptr<T> f = recycler->fill();
    UnexpectedEq(f.use_count(), 1, "reference count");
    recycler->queue(7, f);
    recycler->queue(45, f);
    EXPECT_TRUE(recycler->contains_key(7));
    EXPECT_TRUE(recycler->contains_key(45));
    UnexpectedEq(f.use_count(), 3, "reference count");
    
    *p = val;

    recycler->queue(10, p); // no time to check if it contains
    UnexpectedEq(p.use_count(), 2, "reference count");

    check.join();

    EXPECT_FALSE(waiting_unsafe) <<
        "Thread did not end and join correctly";
}