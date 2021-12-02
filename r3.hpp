#ifndef R3_H
#define R3_H

#include <vector>
#include <queue>
#include <complex>
#include <atomic>
#include <memory>
#include <unistd.h>

#include <iostream>

template <typename T>
class recycle_memory;

// The base class for the pointer will need to be extendable to ALL types that
// the memory system will use
// NOTE: this is a very shallow class, as the memory management is going to be
// dealing with handing the pointer to threads


template <typename T>
struct reuseable_buffer {
    // TODO: array indexing operations

    // TODO: check whether this kills the memory in the shared pointer while we are at it
    T* ptr; // this could be a shared_pointewr
    std::vector<size_t> shape;

    reuseable_buffer(std::vector<size_t> s, T* p, recycle_memory<T>& r) : recycle(r) {
        shape = s;
        ptr = p;
    }

    // TODO: is it needed to explicitly call the shared_ptr destructor?
    ~reuseable_buffer() {
        recycle._return_memory(ptr);
    };

    private:
        recycle_memory<T>& recycle;
};

/* recycle_memory class will both MAKE and DESTROY memory that is within the reuseable_buffer class
 *
 * recycle_memory needs to store unused memory for a particular type
 * needs to store queues for threads to take from
 */
template <typename T>
class recycle_memory {
    // has a vector of recycle_memory struct pointers.
    private:
        std::vector<size_t> shape;
        std::queue<std::shared_ptr<reuseable_buffer<T>>> change;
        std::queue<T*> free;

    public:
        /*
         * Instantiator takes in list of all type signatures with their shapes
         * (and maybe names - might require names actually)
         *
         * for each type, expect:
         *     type
         *     shape
         *     maximum number (if applicable)
         *     constructor
         *     destructor
         */
        recycle_memory(std::vector<size_t> s, unsigned int max) {
            change = std::queue<std::shared_ptr<reuseable_buffer<T>>>();
            free = std::queue<T*>();
            shape = s;

            // make all the new ints here to put in the free list
            for (unsigned int i = 0; i < max; i++) {
                T* temp = new T();
                free.push(temp);
            }
        }
        /*
         * Destructor must destroy ALL memory that was allocated using their
         * corresponding destructors
         */
        ~recycle_memory() {
            // TODO: make sure all buffers have released their memory to free

            while (!free.empty()) {
                T* temp = free.front(); 
                free.pop();
                if (temp != NULL) {
                    delete temp;
                }
            }
        }

        /* get a shared pointer to the buffer we want to fill with data */
        std::shared_ptr<reuseable_buffer<T>> fill() {

            // if free list is empty, wait for memory to be freed
            while (free.empty()) {
                sleep(0.1);
            }
            // else take from free list
            T* ptr = free.front();
            free.pop();

            // make reuseable_buffer for the buffer
            reuseable_buffer<T>* r = new reuseable_buffer<T>(shape, ptr, *this);
            std::shared_ptr<reuseable_buffer<T>> shp(r);
            return shp;

        }

        /* give a shared pointer back to be queued for operation */
        void queue(std::shared_ptr<reuseable_buffer<T>> ptr) {
            change.push(ptr);
            return;
        }

        /* get a shared pointer from the queue to operate on */
        std::shared_ptr<reuseable_buffer<T>> operate() {
            // take a reuseable_buffer off the queue - block until we have one
            // TODO: need to make these queues locking for safety
            while (change.empty()) {
                sleep(0.1);
            }
            std::shared_ptr<reuseable_buffer<T>> r = change.front();
            change.pop();

            return r;
        }

        void _return_memory(T* p) {
            free.push(p);
            return;
        }

        int _free_size() {
            return free.size();
        }

        int _queue_size() {
            return change.size();
        }
};

// TODO: make memory_collection: variadic template for as many buffer types as we want.

#endif
