
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
    static void dec_buffer_ref_count(
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
};

#include "test_mail_impl.hpp"

#endif