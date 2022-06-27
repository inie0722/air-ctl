#include <iostream>
#include <thread>
#include <cstddef>
#include <vector>

#include <gtest/gtest.h>

extern "C"
{
#include "CTL/lockfree/spsc_queue.h"
}

constexpr size_t COUNT = 10000;

constexpr size_t MAX_SIZE = 64;

constexpr size_t THREAD_WRITE_NUM = 1;

constexpr size_t THREAD_READ_NUM = 1;

class verify
{
public:
    template <size_t DATA_SIZE>
    struct value
    {
        size_t val;
        char _[DATA_SIZE - sizeof(val)];
    };

    template <size_t DATA_SIZE>
    void run_one()
    {
        using value_t = value<DATA_SIZE>;

        CTL_lockfree_spsc_queue queue;
        CTL_lockfree_spsc_queue_new(&queue, MAX_SIZE, sizeof(value_t));

        std::vector<size_t> array(COUNT);

        std::chrono::nanoseconds write_diff;
        std::chrono::nanoseconds read_diff;

        std::thread write_thread[THREAD_WRITE_NUM];
        std::thread read_thread[THREAD_READ_NUM];

        for (size_t i = 0; i < THREAD_WRITE_NUM; i++)
        {
            write_thread[i] = std::thread([&]()
                                          {
                value_t data;

                auto start = std::chrono::steady_clock::now();
                for (size_t i = 0; i < COUNT; i++)
                {
                    data.val = i;

                    while (!CTL_lockfree_spsc_queue_push(&queue, &data, 1))
                        std::this_thread::yield();
                }
                auto end = std::chrono::steady_clock::now();
                write_diff = end - start; });
        }

        for (size_t i = 0; i < THREAD_READ_NUM; i++)
        {
            read_thread[i] = std::thread([&]()
                                         {
               value_t data;

                auto start = std::chrono::steady_clock::now();
                for (size_t i = 0; i < COUNT; i++)
                {
                    while(!CTL_lockfree_spsc_queue_pop(&queue, &data, 1))
                        std::this_thread::yield();
                    
                    size_t index = data.val;
                    array[index]++;
                }
                auto end = std::chrono::steady_clock::now();
                read_diff = end - start; });
        }

        for (size_t i = 0; i < THREAD_WRITE_NUM; i++)
        {
            write_thread[i].join();
        }

        for (size_t i = 0; i < THREAD_READ_NUM; i++)
        {
            read_thread[i].join();
        }

        size_t max = 0;
        size_t min = 0;
        for (size_t i = 0; i < COUNT; i++)
        {
            if (array[i] != THREAD_WRITE_NUM)
            {
                if (array[i] > THREAD_WRITE_NUM)
                    max++;
                else
                    min++;
            }
        }

        printf("size/%lu byte\t w/%lu ns\t r/%lu ns\n", DATA_SIZE, write_diff.count() / COUNT, read_diff.count() / COUNT);

        ASSERT_TRUE(max == 0);
        ASSERT_TRUE(min == 0);

        CTL_lockfree_spsc_queue_delete(&queue);
    }

    template <size_t... DATA_SIZE_>
    void run()
    {
        (run_one<DATA_SIZE_>(), ...);
    }
};

TEST(spsc_queue, spsc_queue)
{
    CTL_lockfree_spsc_queue queue;
    CTL_lockfree_spsc_queue_new(&queue, 64, sizeof(int));

    int a = rand();
    CTL_lockfree_spsc_queue_push(&queue, &a, 1);
    ASSERT_TRUE(CTL_lockfree_spsc_queue_size(&queue) == 1);
    ASSERT_FALSE(CTL_lockfree_spsc_queue_empty(&queue));

    int b = 0;
    CTL_lockfree_spsc_queue_pop(&queue, &b, 1);
    ASSERT_TRUE(b == a);
    ASSERT_TRUE(CTL_lockfree_spsc_queue_size(&queue) == 0);
    ASSERT_TRUE(CTL_lockfree_spsc_queue_empty(&queue));

    int array_a[5] = {rand(), rand(), rand(), rand(), rand()};
    CTL_lockfree_spsc_queue_push(&queue, &array_a, 5);
    ASSERT_TRUE(CTL_lockfree_spsc_queue_size(&queue) == 5);
    ASSERT_FALSE(CTL_lockfree_spsc_queue_empty(&queue));

    int array_b[5] = {0};
    CTL_lockfree_spsc_queue_pop(&queue, &array_b, 5);
    ASSERT_TRUE(b == a);
    ASSERT_TRUE(CTL_lockfree_spsc_queue_size(&queue) == 0);
    ASSERT_TRUE(CTL_lockfree_spsc_queue_empty(&queue));

    CTL_lockfree_spsc_queue_delete(&queue);
}

TEST(spsc_queue, performance)
{
    verify v;
    v.run<64, 128, 256, 512, 1024>();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}