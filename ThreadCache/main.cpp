#include"ObjectPool.h"
#include<iostream>
#include<thread>
#include<vector>

const int N = 1000000;
thread_local ObjectPool<double> xgz;
//加上thread_local: 每个线程拥有自己独立的 ObjectPool 副本
// 所有线程同时 New/Delete，互不干扰，不需要加锁

void test(int id)
{
    
    std::vector<double *> myn(N);
    for (int i = 0; i < N;i++)
    {
        myn[i] = xgz.New();
        *myn[i] = id * 1.0;
    }
    for (int i = 0; i < N;i++)
    {
        if(*myn[i]!=id*1.0)
        {
            std::cout << id << "的" << i << "出问题了" << std::endl;
            return;
        }
    }
    for (int i = 0; i < N;i++)
    {
        xgz.Delete(myn[i]);
    }
    std::cout << id << "没问题" << std::endl;
}
int main()
{
    std::cout << "CPU 核心数: " << std::thread::hardware_concurrency() << std::endl;

    std::thread t1(test, 1);
    std::thread t2(test, 2);
    std::thread t3(test, 3);
    std::thread t4(test, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
