
#ifndef test_mail_class_H
#define test_mail_class_H

#include <vector>
#include <memory>

#include "../lender.hpp"

/* Unit testing class - for friending purposes */
struct mail_test {
    mail_test() {}

    template <typename T>
    static void make_mailbox(std::vector<size_t> shape, int max);

    template <typename T>
    static void  mailbox_queue_one(
        std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void mailbox_queue_multi_buffer(
        std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void  mailbox_multi_queue_one_buffer(
        std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void mailbox_read_single_entry_map(
        std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void mailbox_read_multi_entry_map(
        std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void mailbox_read_multi_entry_one_buffer(
    std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void mailbox_multi_read_single_entry_map(
        std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void mailbox_multi_read_multi_entry_map(
        std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void mailbox_multi_read_multi_entry_one_buffer(
    std::shared_ptr<mailbox<T>> recycler
    );

    template <typename T>
    static void wait_read_single_entry (
        std::shared_ptr<mailbox<T>> recycler,
        T val
    );

    template <typename T>
    static void wait_read_multi_entry (
        std::shared_ptr<mailbox<T>> recycler,
        T val
    );

    template <typename T>
    static void multi_wait_read_single_entry (
        std::shared_ptr<mailbox<T>> recycler,
        T val
    );

    template <typename T>
    static void multi_wait_read_multi_entry (
        std::shared_ptr<mailbox<T>> recycler,
        T val
    );

    template <typename T>
    static void multi_wait_read_diff_entry (
        std::shared_ptr<mailbox<T>> recycler,
        T val
    );

    template <typename T>
    static void extra_read_single_entry (
        std::shared_ptr<mailbox<T>> recycler,
        T val
    );

    template <typename T>
    static void extra_wait_read_single_entry (
        std::shared_ptr<mailbox<T>> recycler,
        T val
    );

    template <typename T>
    static void wait_m_threads_same_key (
        std::shared_ptr<mailbox<T>> recycler,
        int m,
        T val
    );

    template <typename T>
    static void wait_m_threads_diff_key( 
        std::shared_ptr<mailbox<T>> recycler,
        std::vector<int> v,
        int m
    );

    private:
        template <typename T>
        static void fthread_wait_for_mail (
            std::shared_ptr<mailbox<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            std::shared_ptr<std::mutex> m,
            bool &waiting_unsafe,
            int key,
            T data
        );
        template <typename T>
        static void fthread_wait_for_mail_multi (
            std::shared_ptr<mailbox<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            std::shared_ptr<std::mutex> m,
            bool &waiting_unsafe,
            int key,
            T data
        );
        template <typename T>
        static void fthread_extra_wait (
            std::shared_ptr<mailbox<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            std::shared_ptr<std::mutex> m,
            bool &waiting_unsafe,
            int key
        );
        template <typename T>
        static void fthread_wait (
            std::shared_ptr<mailbox<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            std::shared_ptr<std::mutex> m,
            std::shared_ptr<bool> waiting_unsafe,
            int key,
            T data
        );
};

#include "test_mail_impl.hpp"

#endif