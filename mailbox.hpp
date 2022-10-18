#include <vector>
#include <queue>
#include <memory>
#include <new>
#include <set>
#include <mutex>
#include <cstring>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <condition_variable>

#include "lender.hpp"


template <typename T>
mailbox<T>::mailbox(const std::vector<size_t> s, int max, int reads) : 
    recycle_memory<T>::recycle_memory(s, max) 
{
    box = std::unordered_map<int, std::shared_ptr<map_value>>();
    max_read = reads;
}

template <typename T>
bool mailbox<T>::contains_key(int idx) { return box.find(idx) != box.end(); }

template <typename T>
bool mailbox<T>::test_contains_key(int idx) {
    std::unique_lock<std::mutex> guard(box_lock);
    return contains_key(idx);
}

template <typename T>
auto mailbox<T>::fill() -> buffer_ptr<T> {
    // take from free and give to caller

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
        assert(memcmp(ptr, f, sizeof(T)) == 0);
        memset(reinterpret_cast<void*>(ptr), 0, sizeof(T)*this->size);
        delete f;
    #endif

    // make reuseable_buffer for the buffer
    auto sp = buffer_ptr<T>(ptr, *this);
    return sp;
}

template <typename T>
void mailbox<T>::queue (int key, buffer_ptr<T> ptr) {
    // add new entry or fill in old
    std::unique_lock<std::mutex> guard(box_lock);
    #ifndef NODEBUG
        // Make sure the ptr exists in the set
        assert(this->pointers.find(ptr.get()) != this->pointers.end());
    #endif

    if (contains_key(key)) {
        std::shared_ptr<map_value> val = box[key];
        // need to take mutex and inform cd
        std::unique_lock<std::mutex> lock(val->val_lock);
        val->value = ptr;
        if (val->read_count <= 0) {
            box.erase(key);
        }
        val->val_cv.notify_all();

    } else {
        box.emplace(key, std::make_shared<map_value>(max_read, ptr));
        box[key]->val_cv.notify_one();
    }
}

template <typename T>
auto mailbox<T>::operate(int key) -> buffer_ptr<T> {
    // check if entry exists. take value if yes.
    // if no, make new entry and sleep
    std::shared_ptr<map_value> val;
    
    std::unique_lock<std::mutex> guard(box_lock);

    // first add new entry if none exists
    if (!contains_key(key)) {
        box.emplace(key, std::make_shared<map_value>(max_read));
    }
    val = box[key];

    // need to take mutex and inform cd
    std::unique_lock<std::mutex> lock(val->val_lock);
    val->read_count--;
    if (val->read_count == 0 && val->value) {
        box.erase(key);
    } else if (val->read_count < 0) {
        // in this case, we have a read that is above allowed reads
        throw std::logic_error("Access after deletion");
    }

    guard.unlock();
    if (!val->value) {
        // IF the value isn't yet set, we still have to wait
        while(!val->value){
            val->val_cv.wait(lock);
        }
    }
    
    buffer_ptr<T> v = val->value;
    
    return v;
}
