#pragma once
#include"Span.h"
#include<vector>
#include<mutex>
#include<sys/mman.h>

class PageCache
{
public:
    span* Pushfreelist(size_t k)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if(freelist[k].Emptyspan()==false)
        {
            return freelist[k].Popspan();
        }
        else
        {
            for (int i = k; i < 129;i++)
            {
                if (freelist[i].Emptyspan()==false)
                {
                    span *big = freelist[i].Popspan();
                    span *gei;
                    gei->start = big->start;
                    gei->size = k;

                    big->start = (void*)((char *)(gei->start) + k * 8192);
                    big->size = i - k;

                    freelist[i - k].Pushspan(big);
                    return gei;
                }
            }
            void *na = mmap(nullptr, 128 * 8192, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            


        }
    }
    void Popfreelist(span* n)
    {

    }
private:
    Spanlist freelist[129];
    std::mutex mtx;
};