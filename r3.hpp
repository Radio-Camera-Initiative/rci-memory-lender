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
// dealing with handing the pointer to threads\

//TODO array indexing operations
//todo atomic_shared_ptr -> reference counting managed by the shared pointer.
template <typename T>
struct Reuse { // -> override so that ppl can index into this (but really index into the buffer by the pointer)
// TODO: why not just a std::shared_ptr? -> need shape?
    std::atomic<std::shared_ptr<T>> ptr; // -> make atomic shared pointer -> pass deleter in Recycle
    std::vector<size_t> shape;

    Reuse(std::atomic<std::shared_ptr<T>> p, std::vector<size_t> s) {
        ptr = p;
        shape = s;
    }

// TODO: up the ref count with copy constructor
    Reuse(const Reuse<T> &r) {
        ptr = std::atomic(r->ptr);
    }

    /* TODO: need to decrement with destruction
     */
    ~Reuse() {};
};

/* Recycle class will both MAKE and DESTROY memory that is within the Reuse class
 *
 * Recycle needs to store unused memory for a particular type
 * needs to store queues for threads to take from
 */
// TODO: will need to convert this into a variadic template
template <typename T>
class Recycle {
    // has a vector of Recycle struct pointers.
    private:
        std::vector<size_t> shape;
        std::queue<Reuse<T>> operate;
        std::queue<T*> free;

        /* Deleter for pointer */
        void reuse_deleter(T* ptr) {
            // we don't delete the data, only pass back
            free.push(ptr);
        }

    public:
        typedef void(*func)(Recycle<T>& r3, Reuse<T>* r);

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
        Recycle(std::vector<size_t> s, unsigned int max) {
            std::queue<T> operate();
            std::queue<T> free();
            shape = s;
        }
        /*
         * Destructor must destroy ALL memory that was allocated using their
         * corresponding destructors
         */
        ~Recycle() {
        }

        /* use a name to get the pointer that we want to start filling */
        Reuse<T> new_ptr() {
            // make shared pointer
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

            std::atomic<std::shared_ptr<T>> share_ptr(ptr, reuse_deleter);

            // make Reuse for the buffer, increment the count and return it
            Reuse<T> r = Reuse<T>(share_ptr, shape); // TODO: new?
            return r;

        }

        /* give a pointer back to be queued for operation */
        void queue_ptr(Reuse<T> atomic_shared_ptr) {
            // decrement the Reuse count and put it in the operation queue
            // atomic_shared_ptr->read--;
            operate.push(atomic_shared_ptr);
            return;
        }

        /* use a name to give a pointer back to be operated on */
        // TODO: how does this template if we don't have a particular type?
        //       best I can come up with is use T but always pass a nullptr?
        Reuse<T> start_operation() {
            // take a Reuse off the queue - block until we have one
            //TODO: need to make these queues locking for the mutex stuff
            while (operate.empty()) {
                sleep(0.1);
            }
            Reuse<T> r = operate.front();
            operate.pop();

            // increment the Reuse and pass to the caller
            // r->read++;
            return r;
        }

        /* end a threads operation on the pointer whether it be read or write */
        // void end_operation(Reuse<T>* atomic_shared_ptr, std::vector<func>* read_threads) { // TODO: the threads also need to have the function passed?
        //     // for each read_thread increment the atomic ref count and pass the ptr
        //     /* read_threads can and SHOULD be null for a reader thread getting rid of
        //      * their pointer */
        //     if (read_threads != NULL) {
        //         for(typename std::vector<func>::iterator iter = read_threads->begin();
        //             iter != read_threads->end();
        //             iter++
        //         ) {
        //             func f = *iter;
        //             // atomic_shared_ptr->read++;
        //             std::thread th (f, std::ref(*this), atomic_shared_ptr); // start the thread with the pointer
        //             // TODO: the thread needs to get a const - but how is that const passed back to this function?
        //         }
        //     }
        //
        //     // decrement the atomic_shared_ptr for the caller
        //     // atomic_shared_ptr->read--;
        //
        //     // if ref count 0 then collect Reuse class and add buffer to free list
        //     if (atomic_shared_ptr->read == 0) {
        //         free.push(atomic_shared_ptr->ptr);
        //         // delete Reuse class made for this
        //         delete atomic_shared_ptr;
        //     }
        //     return;
        // }
};

#endif
