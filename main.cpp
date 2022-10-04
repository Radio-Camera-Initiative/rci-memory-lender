 // main.cpp
#include <vector>
#include <thread>
#include <iostream>
#include "lender.hpp"

/* ask class for buffers and hand them back */
void make_vis(library<int>& r3) {
    //TODO: make multiple buffers test
    buffer_ptr<int> r = r3.fill();
    *r = 5;
    r3.queue(r);
}

/* read from visibility and write to disk (just write to terminal right now) */
void read_vis(buffer_ptr<int> vis) {
    std::cout << "vis:" << *vis << std::endl;
}

/* ask for visibility and make thread to read */
void change_vis(library<int>& r3) {
    // what is condition to stop looping this operation and exit gracefully?
    buffer_ptr<int> vis = r3.operate();
    int i = *vis;
    *vis = i+1;
    std::thread reading(read_vis, vis);

    reading.join();
}

void test_int() {
    // make the class
    std::vector<size_t> shape = std::vector<size_t>(1, 1); // n copies of i (n, i)
    int max = 1;
    library<int> r3(shape, max); 
    // std::cout << r3.private_free_size() << std::endl;


    // make a thread to be making visibilities
    std::thread making (make_vis, std::ref(r3));

    // make a thread to change visibility
    std::thread changing (change_vis, std::ref(r3));

    making.join();
    changing.join();
    
    // std::cout << r3.private_free_size() << std::endl;
}

void test_array() {
    std::vector<size_t> shape = std::vector<size_t>(2, 2); // n copies of i (n, i)
    int max = 1;
    library<int> recycler(shape, max); 
    auto buffer = recycler.fill();

    size_t size = 1;
    for (auto iter = shape.begin(); iter != shape.end(); iter++) {
        size *= *iter;
    }

    // fill each index with its number
    int i = 0;
    for (size_t idx = 0; idx < size; idx++, i++) {
        buffer[idx] = i;

        std::cout << "Expected index " + std::to_string(idx) +
            " to be " + std::to_string(i) + " and got " + 
            std::to_string(buffer[idx]) << std::endl;
    }
}

void test_debug() {
    std::vector<size_t> shape = std::vector<size_t>(2, 2); // n copies of i (n, i)
    int max = 1;
    library<int> recycler(shape, max); 
    size_t size = 1;
    for (auto iter = shape.begin(); iter != shape.end(); iter++) {
        size *= *iter;
    }
    // size *= sizeof(int);
    std::cout << sizeof(int) << std::endl;
    int* raw_ptr;
    {
        auto buffer = recycler.fill();
        raw_ptr = buffer.get();

        for (size_t ci = 0; ci < size; ci++) {
            std::cout << raw_ptr[ci] << std::endl;
        }
    }

    std::cout << std::dec << raw_ptr << std::endl;
    
    for (size_t ci = 0; ci < size; ci++) {
        std::cout << std::hex << raw_ptr[ci] << std::endl;
    }

    int* f = new int();
    memset(f, 0xf0, sizeof(int));
    std::cout << memcmp(f, raw_ptr, sizeof(int)) << std::endl;
    free(f);
    
}

// #include "testing/test_class.hpp"
#include <numeric>

template <typename T>
void thread_take_and_release(
    std::shared_ptr<library<T>> recycler,
    bool& end_condition
) {
    while(true) {
        std::cout << "thread " << std::this_thread::get_id() << std::endl;
        auto p = recycler->operate();
        std::cout << "taking buffer" << std::endl;
        if (p.kill()) {
            std::cout << "killing " << std::this_thread::get_id() << std::endl;
            recycler->queue(p.poison_pill());
            break;
        }
        *p = 0;
    }
}

void testing_pipeline() {
    int max = 5;
    std::vector<size_t> shape = std::vector<size_t>(1, 1);
    std::shared_ptr<library<int>> recycler = 
        std::make_shared<library<int>>(shape, max*2); 
    std::vector<int> filling(30);
    std::iota(std::begin(filling), std::end(filling), 0);
    bool end_condition = false;
    std::vector<std::thread> threads(max); // make m
    for (auto& i : threads) {
        i = std::thread(
            thread_take_and_release<int>, recycler, std::ref(end_condition)
        );
    }
    for(auto i : filling) {
        // fill
        auto p = recycler->fill();
        *p = i;
        std::cout << "writing " << std::to_string(*p) << std::endl;
        recycler->queue(p);
    }
    auto p = recycler->fill();
    std::cout << p.kill() << std::endl;
    auto q = p.poison_pill();
    std::cout << "q " << q.kill() << std::endl;
    std::cout << p.kill() << std::endl;
    recycler->queue(p);
    // end_condition = true;
    for (auto& i : threads) {
        i.join();
    }
    // take the poison pill off to let the recycler clean everything up properly
    p = recycler->operate();
}

int main(int argc, char** argv) {
    test_int();
    test_array();
    test_debug();
    testing_pipeline();

    // TODO: how does the main thread not end before all the sub threads from
    //    the operating threads end
    std::cout << "end" << std::endl;

    std::vector<size_t> shape = std::vector<size_t>(2, 2); // n copies of i (n, i)
    int max = 1;
    mailbox<int> recycler(shape, max); 

    buffer_ptr<int> r = recycler.fill();
    read_vis(r);
    *r = 7;
    read_vis(r);
    recycler.queue(1, r);
    std::cout << "queued" << std::endl;
    auto s = recycler.operate(1);
    read_vis(s);
}
