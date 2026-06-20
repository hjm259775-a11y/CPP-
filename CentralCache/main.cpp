#include<iostream>
#include"CentralCache.h"


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

    return 0;
}