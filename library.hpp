#include <vector>
#include <queue>
#include <memory>
#include <new>
#include <set>
#include <mutex>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <condition_variable>

#include "lender.hpp"

template <typename T>
library<T>::library(const std::vector<size_t> s, unsigned int max) : 
    recycle_memory<T>::recycle_memory(s, max), change_q(std::deque<buffer_ptr<T>>())
{}

template <typename T>
auto library<T>::fill() -> buffer_ptr<T> {

    std::unique_lock<std::mutex> lock(this->free_mutex);
    while(this->free_q.empty()) {  
        this->free_variable.wait(lock);
    }

    // take from free list
    T* ptr = this->free_q.front();
    this->free_q.pop_front();

    #ifndef NODEBUG
        // check there was no changes after free
        T* f = new T();
        memset(reinterpret_cast<void*>(f), 0xf0, sizeof(T));
        std::clog << memcmp(ptr, f, sizeof(T)) << " and expecting 0" << std::endl;
        std::clog << (memcmp(ptr, f, sizeof(T)) == 0) << std::endl;
        assert(memcmp(ptr, f, sizeof(T)) == 0);
        memset(reinterpret_cast<void*>(ptr), 0, sizeof(T)*this->size);
        delete f;
        std::clog << "end of lib fill debug" << std::endl;
    #endif

    // make reuseable_buffer for the buffer
    auto bp = buffer_ptr<T>(ptr, *this);
    return bp;

}

template <typename T>
void library<T>::queue(buffer_ptr<T> ptr) {
    std::unique_lock<std::mutex> guard(change_mutex);
    #ifndef NODEBUG
        // Make sure the ptr exists in the set
        assert(this->pointers.find(ptr.get()) != this->pointers.end());
    #endif
    change_q.push_back(ptr);
    change_variable.notify_one();
}

template <typename T>
auto library<T>::operate() -> buffer_ptr<T> {

    std::unique_lock<std::mutex> lock(change_mutex);
    while (change_q.empty()) {
        change_variable.wait(lock);
    }

    buffer_ptr<T> r = change_q.front();
    change_q.pop_front();

    return r;
}

template <typename T>
auto library<T>::change_condition() -> bool {
    return !change_q.empty();
}


template <typename T>
auto library<T>::private_queue_size() -> int {
    return change_q.size();
}