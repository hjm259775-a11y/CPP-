#pragma once
#include"Span.h"
#include<vector>
#include<mutex>
#include<sys/mman.h>
#include<cassert>

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
            for (int i = k + 1; i < 129; i++)
            {
                if (freelist[i].Emptyspan()==false)
                {
                    span *big = freelist[i].Popspan();
                    span *gei = new span;
                    gei->start = big->start;
                    gei->size = k;

                    big->start = (void *)((char *)(gei->start) + k * 8192);
                    big->size = i - k;

                    freelist[i - k].Pushspan(big);
                    return gei;
                }
            }
            void *na = mmap(nullptr, 128 * 8192, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            span *fang = new span;
            fang->start = na;
            fang->size = 128;
            freelist[128].Pushspan(fang);
        }
        for (int i = k + 1; i < 129; i++)
        {
            if (freelist[i].Emptyspan()==false)
            {
                span *big = freelist[i].Popspan();
                span *gei = new span;
                gei->start = big->start;
                gei->size = k;

                big->start = (void*)((char *)(gei->start) + k * 8192);
                big->size = i - k;

                freelist[i - k].Pushspan(big);
                return gei;
            }
        }
        return nullptr;
    }
    void Popfreelist(span* n)
    {
        std::lock_guard<std::mutex> lock(mtx);
        freelist[n->size].Pushspan(n);
    }
    

private:
    Spanlist freelist[129];
    std::mutex mtx;
};
