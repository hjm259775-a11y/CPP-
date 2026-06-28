#pragma once
#include"Span.h"
#include<mutex>
#include<sys/mman.h>

class PageCache
{
public:
    static PageCache& GetInstance()
    {
        static PageCache inst;
        return inst;
    }

    span *Newspan(size_t k)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (freelist[k].Empty())
        {
            for (int i = k + 1; i < 129; i++)
            {
                if(freelist[i].Empty()==false)
                {
                    span *big = freelist[i].popfront();

                    span *gei = new span;
                    gei->start = big->start;
                    gei->size = k;
                    gei->_is = true;

                    big->start = (void*)((char*)big->start + k * 8192);
                    big->size = i - k;
                    big->_is = false;

                    freelist[i - k].pushfront(big);
                    return gei;
                }
            }
            void *na = mmap(nullptr, 128 * 8192, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            span *fang = new span;
            fang->start = na;
            fang->size = 128;
            fang->_is = false;
            freelist[128].pushfront(fang);

            for (int i = k + 1; i < 129; i++)
            {
                if(freelist[i].Empty()==false)
                {
                    span *big = freelist[i].popfront();
                    span *gei = new span;
                    gei->start = big->start;
                    gei->size = k;
                    gei->_is = true;

                    big->start = (void*)((char*)big->start + k * 8192);
                    big->size = i - k;
                    big->_is = false;

                    freelist[i - k].pushfront(big);
                    return gei;
                }
            }
        }
        else
        {
            return freelist[k].popfront();
        }
        return nullptr; // 实际不会走到这里
    }

    void DeleteSpan(span *n)
    {
        std::lock_guard<std::mutex> lock(mtx);
        n->_is = false;
        freelist[n->size].pushfront(n);
    }

private:
    PageCache() {}
    PageCache(const PageCache&) = delete;
    PageCache& operator=(const PageCache&) = delete;

    SpanList freelist[129];
    std::mutex mtx;
};
