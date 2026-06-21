#include<iostream>
#include<thread>
#include"CentralCache.h"


CentralCache xgz;

void worker(int id)
{
    void *a = malloc(24);
    void *b = malloc(24);
    void *c = malloc(24);
    *(void **)a = b;
    *(void **)b = c;
    *(void **)c = nullptr;

    xgz.ReleaseRange(a, 24, 3);

    void *head = xgz.FetchRange(24, 2, 0);
    void *second = *(void **)head;

    if(head==c && second==b)
    {
        std::cout << "好!" << std::endl;
    }
}


int main()
{
    void *a = malloc(24);
    void *b = malloc(24);
    void *c = malloc(24);
    *(void **)a = b;
    *(void **)b = c;
    *(void **)c = nullptr;

    CentralCache xgz;
    xgz.ReleaseRange(a, 24, 3);

    void *head = xgz.FetchRange(24, 2, 0);
    void *second = *(void **)head;

    if(head==c && second==b)
    {
        std::cout << "好" << std::endl;
    }




    std::thread t1(worker, 1);
    std::thread t2(worker, 2);

    t1.join();
    t2.join();

    std::cout << "完成" << std::endl;

    return 0;
}