#include<iostream>
#include<thread>
#include<vector>
#include"ThreadCache.h"

CentralCache centralcache;
thread_local ThreadCache tc;

void worker(int id)
{
    const int N = 1000;
    std::vector<std::pair<void*, size_t>> allocs;

    size_t sizes[] = {8, 16, 32, 64, 128, 256, 512, 1024, 2048};
    for (int i = 0; i < N; i++)
    {
        size_t s = sizes[i % 9];
        void* p = tc.Allocate(s);
        if (!p)
        {
            std::cout << "线程" << id << ": 分配失败!\n";
            return;
        }
        for (size_t j = 0; j < s; j++)
            ((char*)p)[j] = (char)(id + j);
        allocs.push_back({p, s});
    }

    for (auto& a : allocs)
    {
        char* p = (char*)a.first;
        size_t s = a.second;
        for (size_t j = 0; j < s; j++)
            if (p[j] != (char)(id + j))
            {
                std::cout << "线程" << id << " 数据损坏\n";
                return;
            }
    }

    for (auto& a : allocs)
        tc.Deallocate(a.first, a.second);

    std::cout << "线程" << id << " OK\n";
}

int main()
{
    const int NUM_THREADS = 4;
    std::thread threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
        threads[i] = std::thread(worker, i);

    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].join();

    std::cout << "\n所有线程测试通过!\n";
    return 0;
}
