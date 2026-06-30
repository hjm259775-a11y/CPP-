#pragma once
#include"SizeClass.h"
#include<mutex>
#include<vector>
#include"PageCache.h"
class CentralCache
{
public:
    CentralCache()
    {
        for (int i = 0; i < 209;i++)
        {
            _freelist[i] = nullptr;
        }
    }


    void* Popfront(int size,int n)
    {
        std::lock_guard<std::mutex> loxk(mtx);
        int shiji = Index(size);

        if (_freelist[shiji] == nullptr)
        {
            size_t obj = roundup(size);
            size_t pages = (n * obj + 8191) / 8192;

            span *page = pagecache.Pushfreelist(pages);
            char *str = (char*)(page->start);

            size_t sum = pages * 8192;
            size_t central_n = sum / obj;

            for (int i = 0; i < central_n-1;i++)
            {
                *(void **)str = str + obj;
                str += obj;
            }
            *(void **)str = nullptr;
            _freelist[shiji] = page->start;
        }

        
        void *head = _freelist[shiji];
        void *end = head;
        for (int i = 0; i < n && end != nullptr; i++)
        {
            end = *(void **)end;
        }

        if(end==nullptr)
        {
            size_t obj = roundup(size);
            size_t pages = (n * obj + 8191) / 8192;

            span *page = pagecache.Pushfreelist(pages);
            char *str = (char*)(page->start);

            size_t sum = pages * 8192;
            size_t central_n = sum / obj;





            _freelist[shiji] = nullptr;
            return head;
            
        }
        _freelist[shiji] = end;
        return head;
    }

    void Pushfront(int size,int n,void *head)
    {
        std::lock_guard<std::mutex> lock(mtx);
        int shiji = Index(size);
        void *end = head;
        n--;
        while (n--)
        {
            end = *(void **)end;
        }
        *(void **)end = _freelist[shiji];
        _freelist[shiji] = head;
    }

private:
    void* _freelist[208];
    std::mutex mtx;
    PageCache pagecache;//方便调用PageCache
};
