
#ifndef test_class_H
#define test_class_H

#include <vector>
#include <memory>

#include "../r3.hpp"

/* Unit testing class - for friending purposes */
struct unit_test {
    unit_test() {}

    template <typename T>
    static void make_recycle_memory(std::vector<size_t> shape, int max);
    
    template <typename T>
    static void take_one_buffer_from_fill(
        std::shared_ptr<recycle_memory<T>> recycler, 
        std::vector<size_t> shape, int max
    );
    
    template <typename T>
    static void check_buffer_destruction(
        std::shared_ptr<recycle_memory<T>> recycler, 
        int max
    );

    template <typename T>
    static void dec_buffer_ref_count(
        std::shared_ptr<recycle_memory<T>> recycler
    );
    
    template <typename T>
    static void change_one_buffer(
        std::shared_ptr<recycle_memory<T>> recycler, 
        int max, 
        T data
    );
    
    template <typename T>
    static void multi_change_buffer(
        std::shared_ptr<recycle_memory<T>> recycler, 
        int max, 
        T data,
        T diff
    );
    
    template <typename T>
    static void set_buffer_ptr_array (
        std::shared_ptr<recycle_memory<T>> recycler
    );
    
    template <typename T>
    static void queue_buffer_from_fill (
        std::shared_ptr<recycle_memory<T>> recycler, 
        int max,
        T data
    );

    template <typename T>
    static void dec_operate_queue(
        std::shared_ptr<recycle_memory<T>> recycler
    );

    
    template <typename T>
    static void change_buffer_threaded(
        std::shared_ptr<recycle_memory<T>> recycler,
        T data
    );
    
    template <typename T>
    static void multi_change_buffer_threaded(
        std::shared_ptr<recycle_memory<T>> recycler,
        T data[]
    );
    
    template <typename T>
    static void wait_on_fill_threaded(
        std::shared_ptr<recycle_memory<T>> recycler, 
        int max,
        T data
    );
    
    template <typename T>
    static void buffer_from_empty_queue_threaded(
        std::shared_ptr<recycle_memory<T>> recycler, 
        int max,
        T data
    );
    
    template <typename T>
    static void wait_multi_take_from_fill_threaded(
        std::shared_ptr<recycle_memory<T>> recycler, 
        int max,
        T data
    );
    
    template <typename T>
    static void watcher_check_reference_counts(
        std::shared_ptr<recycle_memory<T>> recycler
    );

    private:
        template <typename T>
        static void thread_read(buffer_ptr<T> b, T data);
        template <typename T>
        static void thread_wait_fill(
            std::shared_ptr<recycle_memory<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            bool &waiting_unsafe
        );
        template <typename T>
        static void thread_wait_queue(
            std::shared_ptr<recycle_memory<T>> recycler,
            std::shared_ptr<std::condition_variable> cv,
            bool &waiting_unsafe
        );
        template <typename T>
        static void thread_watcher(
            buffer_ptr<T> p,
            std::shared_ptr<std::condition_variable> cv,
            bool &operating_unsafe,
            int &check_ref
        );


};

#include "test_impl.hpp"

#endif