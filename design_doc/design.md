# Design Doc

## Purpose of code

Reduce memory allocation overhead for a streaming pipeline, and remove the need to move data between blocks of memory as would be needed in a ring-buffer solution.

A scientist writing code would be able to call a high-level object for a certain type (i.e. a visibility type array or a position type array) and receive back a buffer that they can operate on as they normally would, with extra information such as shape included.

The user is expected to write threads that will 1. fill the buffer and 2. operate on the buffer. Getting the buffers to operate on will all happen from a central object (memory_collection). The user is free to give the buffer reference to multiple threads to read from - it is highly discouraged to operate on them from multiple threads.

### Scientist use case

There are three main functions that a scientist will call from the memory_collection. A diagram showing these functions and how they pass around the reuseable_buffers is below:

<img src='https://github.com/Radio-Camera-Initiative/r3_central_manager/blob/main/design_doc/fill_operate_queue.jpg' title='memory_collection functions' width='' alt='memory_collection functions' />

Ideally, concurrency would occur in the operation threads, such that multiple threads can be running the algorithms in the pipeline on each visibility buffer that is in the queue. Concurrency is also able to happen in other places, such as filling the visibility buffers with new data, as each queue will be concurrency safe whether writing to or taking from them.

Here is an example of how a scientist might write a thread to use multiple types of memory buffers:

```
func calibrate_and_grid (memory_collection)
    vis_ptr = memory_collection.operate() // take vis pointer from queue
    flag_ptr = memory_collection.fill() // take flag pointer to fill with data

    do_flagging(vis_ptr, flag_ptr) // indexing into pointer should be possible
    thread.start(write_to_disk, flag_ptr)

    jones_ptr = memory_collection.operate() // another thread solves calibration

    do_apply(vis_ptr, flag_ptr, jones_ptr) // flag & jones as read
    thread.start(write_to_disk, jones_ptr)
    thread.start(write_to_disk, vis_ptr)

    grid_ptr = memory_collections.operate()
    do_gridding(vis_ptr, grid_ptr) // could also be passed to another thread

```

## Classes

`memory_collection` is a variadic template class that will take multiple types and create `recycle_memory` classes for each time. It is a singleton and the class that the scientist will be calling for memory.

`recycle_memory` is a template class that will store atomic queues of memory pointers to be filled with data, and reuseable_buffers for actual thread operation. It has standard metadata for the type that it holds and makes and gives reuseable_buffers to the `memory collection`. This is also a singleton.

`reuseable_buffer` is a template class that stores metadata and the pointer to memory that scientists will be using in their functions/coding.

<img src='https://github.com/Radio-Camera-Initiative/r3_central_manager/blob/main/design_doc/class_hierarchy.jpg' title='Class Hierarchy' width='' alt='Class Hierarchy' />

### Pseudocode

```
memory_collections (variadic)
    memory<T> = recycle_memory<T> for each Type given

    Parametrized constructor (<T t, shape, max>, <U u, shape, max> ...)

    T new_buffer (T) {
        return memory<T>.new_buffer
    }

    void complete_buffer (T) {
        memory<T>.complete_buffer(T)
    }

    T operate_buffer (T) {
        return memory<T>.operate_buffer
    }
```

```
reusable_buffer<T> (array indexable)
    ptr = atomic_shared_ptr to the underlying memory buffer
    shape = array/list of dimensions
    manager = recycle_memory object

    Default constructor
    Parametrized constructor(T* p, s, &m) {
        shape = s
        manager = m
        new atomic_shared_ptr(p) -> to increment the reference count
    }
    Copy-constructor {
        new atomic_shared_ptr to increment reference count
        copy shape
    }
    Destructor {
        delete atomic_shared_ptr to decrement reference count
        -> if reference count is 0 then pass the buffer pointer back to manager's free list
    }
```

```
recycle_memory<T>
    free = bounded atomic queue of pointers to memory (see Boost PriorityQueue)
    operate = bounded atomic queue of reuseable_buffers ("")
    shape = array/list of dimensions
    count = # of memory buffers to keep track of
    max = max number of buffers allowed for this type

    Default constructor
    Parametrized constructor (max, shape) {
        free = PriorityQueue(max)
        operate = PriorityQueue(max)
    }
    Copy constructor = delete because this should not be copied.
    Destructor {
        if !operate.empty() then destruct all elements on queue (destructor should place elements into free)
        if free.count < count then wait for all buffers to finish reading/writing
        for each buffer in free queue, free that memory.
    }

    new_buffer { -> called by high-level system
        if !free.empty() {
            free.lock
            reuseable_buffer(free.front(), shape, *this) -> pass self by reference (do not want to copy)
            free.pop()
            free.unlock
            return reuseable_buffer
        } else if (count < max) {
            p = new T (or allocate memory another way)
            reusable_buffer(p, shape, *this)
            count++
            return reuseable_buffer
        } else {
            wait for a buffer to be freed up
        }
    }

    complete_buffer(reuseable_buffer) { // called only by buffer filling thread -> these can all be copy constructors because the shared atomic pointer will increment and decrement reference counts accordingly, as long as operate keeps a shared_atomic_ptr alive
        while operate.size == max {
            sleep(0.1) to wait for a buffer
        }
        operate.lock
        operate.push(reusable_buffer)
        operate.unlock
    }

    operate_buffer {
        while operate.empty() {
            sleep(0.1) to wait for a buffer
        }
        operate.lock
        reuseable_buffer = operate.front()
        operate.pop()
        operate.unlock
        return reusable_buffer // can be copy constructor -> the recycle_memory pointer will go out of scope to reduce ref count
    }
```
