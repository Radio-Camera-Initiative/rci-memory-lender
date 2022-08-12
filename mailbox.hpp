#include <vector>
#include <queue>
#include <memory>
#include <new>
#include <set>
#include <mutex>
#include <cstring>
#include <cassert>
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
auto mailbox<T>::fill() -> buffer_ptr<T> {
    // take from free and give to caller

    std::unique_lock<std::mutex> lock(this->free_mutex);
    while(this->free_q.empty()) {  
        this->free_variable.wait(lock);
    }

    // take from free list
    T* ptr = this->free_q.front();
    this->free_q.pop_front();

    // make reuseable_buffer for the buffer
    auto sp = buffer_ptr<T>(ptr, *this);
    return sp;
}

template <typename T>
void mailbox<T>::queue (int key, buffer_ptr<T> ptr) {
    // add new entry or fill in old
    std::unique_lock<std::mutex> guard(box_lock);

    if (contains_key(key)) {
        // need to take mutex and inform cd
        std::unique_lock<std::mutex> lock(box[key]->val_lock);
        box[key]->value = ptr;
        box[key]->count = max_read;
        box_cv.notify_one();

    } else {
        std::shared_ptr<map_value> val = std::make_shared<map_value>();
        std::unique_lock<std::mutex> lock(val->val_lock);

        val->value = ptr;
        val->count = max_read;
        box.emplace(key, val);
        
    }
}

template <typename T>
auto mailbox<T>::operate(int key) -> buffer_ptr<T> {
    // check if entry exists. take value if yes.
    // if no, make new entry and sleep
    std::unique_lock<std::mutex> guard(box_lock);

    if (contains_key(key)) {
        // need to take mutex and inform cd
        std::unique_lock<std::mutex> lock(box[key]->val_lock);
        if (!box[key]->value) {
            // IF the value isn't yet set, we still have to wait
            lock.unlock();
            while(!box[key]->value){
                box_cv.wait(guard);
            }
            std::unique_lock<std::mutex> lock1(box[key]->val_lock);
        }
        box[key]->count--;
        buffer_ptr<T> v = box[key]->value;
        if (box[key]->count == 0) {
            box.erase(key);
        }
        // must notify in case other people are waiting
        box_cv.notify_one();
        return v;

    } else {
        std::shared_ptr<map_value> val = std::make_shared<map_value>();
        std::unique_lock<std::mutex> lock(val->val_lock);
        val->value = NULL;
        // val.count is initialized with the writer

        box.emplace(key, val);

        lock.unlock();
        while(!val->value){
            box_cv.wait(guard);
        }
        std::unique_lock<std::mutex> lock1(val->val_lock);

        val->count--;
        buffer_ptr<T> v = val->value;
        if (val->count == 0) {
            box.erase(key);
        }
        box_cv.notify_one();
        return v;
    }
}