#ifndef R3_H
#define R3_H

#include <vector>
#include <queue>
#include <complex>
#include <atomic>
#include <memory>
#include <unistd.h>

// The base class for the pointer will need to be extendable to ALL types that
// the memory system will use
// NOTE: this is a very shallow class, as the memory management is going to be
// dealing with handing the pointer to threads

//TODO array indexing operations
//todo atomic_shared_ptr -> reference counting managed by the shared pointer.
template <typename T>
struct reuseable_buffer { // -> override so that ppl can index into this (but really index into the buffer by the pointer)
    std::shared_ptr<T> ptr;
    std::vector<size_t> shape;

    reuseable_buffer(std::vector<size_t> s, T* p) {
        shape = s;
        ptr = std::shared_ptr<T>(p);
    }

    ~reuseable_buffer() {};
};

/* recycle_memory class will both MAKE and DESTROY memory that is within the reuseable_buffer class
 *
 * recycle_memory needs to store unused memory for a particular type
 * needs to store queues for threads to take from
 */
// TODO: will need to convert this into a variadic template
template <typename T>
class recycle_memory {
    // has a vector of recycle_memory struct pointers.
    private:
        std::vector<size_t> shape;
        std::queue<reuseable_buffer<T>*> change;
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
            std::queue<T> change();
            std::queue<T> free();
            shape = s;
        }
        /*
         * Destructor must destroy ALL memory that was allocated using their
         * corresponding destructors
         */
        ~recycle_memory() {
        }

        /* use a name to get the pointer that we want to start filling */
        reuseable_buffer<T>* fill() {
            // empty ptr
            T* ptr;

            // if free list is empty, make a new buffer
            if (free.empty()) {
                // making a new buffer might use a type-specific constructor
                ptr = new T;
            } else {
                // else take from free list
                ptr = free.front();
                free.pop();
            }

            // make reuseable_buffer for the buffer
            reuseable_buffer<T>* r = new reuseable_buffer<T>(shape, ptr);
            return r;

        }

        /* give a pointer back to be queued for operation */
        void queue(reuseable_buffer<T>* share_ptr) {
            change.push(share_ptr);
            return;
        }

        /* use a name to give a pointer back to be operated on */
        // TODO: how does this template if we don't have a particular type?
        //       best I can come up with is use T but always pass a nullptr?
        reuseable_buffer<T>* operate() {
            // take a reuseable_buffer off the queue - block until we have one
            //TODO: need to make these queues locking for the mutex stuff
            while (change.empty()) {
                sleep(0.1);
            }
            reuseable_buffer<T>* r = change.front();
            change.pop();

            return r;
        }

};

#endif
