#include<mutex>
#include"SizeClass.h"



class CentralCache
{
public:
    CentralCache()
    {
        for (int i = 0; i < 200;i++)
        {
            _freelist[i] = nullptr;
        }
    }

    void* FetchRange(int size,int n,int actualnum)
    {
        int slot = Index(size);
        mtx.lock();

        void *head = _freelist[slot];
        void *cur = head;
        actualnum = 0;

        while(cur!=nullptr && actualnum<n)
        {
            cur = *(void **)cur;
            actualnum++;
        }

        if(actualnum>0)
        {
            _freelist[slot] = cur;
        }
        else
        {
            head = nullptr;
        }

        mtx.unlock();
        return head;
    }
    void ReleaseRange(void* start,size_t size,size_t num)
    {
        int slot = Index(size);
        mtx.lock();

        void *cur = start;
        for (int i = 0; i < num;i++)
        {
            void *next = *(void **)cur;
            *(void **)cur = _freelist[slot];
            _freelist[slot] = cur;
            cur = next;
        }
        mtx.unlock();
    }

private:
    void *_freelist[200];
    std::mutex mtx;
};