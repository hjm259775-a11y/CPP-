#include"Span.h"
#include<mutex>


class PageCache
{
public:
    PageCache(){}
    span *Newspan(size_t k)
    {
        if(freelist[k].Empty())
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
                    //扔给用户的

                    big->start = big->start + k * 8192;
                    big->size = i - k;
                    big->_is = false;
                    //返回链表的

                    freelist[i - k].pushfront(big);

                    return gei;
                }
            }
            void* 
        }
        else
        {
            return freelist[k].popfront();
        }
    }
    void DeleteSpan(span *n)
    {
        
    }
    

private:
    SpanList freelist[129];
    std::mutex mtx;
};
