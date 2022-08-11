
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
    static void take_one_buffer_from_fill(
        std::shared_ptr<mailbox<T>> recycler, 
        std::vector<size_t> shape, int max
    );
    
    template <typename T>
    static void check_buffer_destruction(
        std::shared_ptr<mailbox<T>> recycler, 
        int max
    );

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
    static void change_one_buffer(
        std::shared_ptr<mailbox<T>> recycler, 
        int max, 
        T data
    );
    
    template <typename T>
    static void multi_change_buffer(
        std::shared_ptr<mailbox<T>> recycler, 
        int max, 
        T data,
        T diff
    );
    
    template <typename T>
    static void set_buffer_ptr_array (
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

    private:
        template <typename T>
        static void wait_for_mail (
            std::shared_ptr<mailbox<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            bool &waiting_unsafe,
            int key,
            T data
        );
        template <typename T>
        static void multi_wait_for_mail (
            std::shared_ptr<mailbox<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            bool &waiting_unsafe,
            int key,
            T data
        );
};

#include "test_mail_impl.hpp"

#endif