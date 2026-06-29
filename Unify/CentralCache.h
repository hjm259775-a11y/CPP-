#pragma once
#include<mutex>
#include"SizeClass.h"
#include"PageCache.h"
const size_t MAX_BATCH = 32;
const size_t FREELIST_SIZE = 208;

class CentralCache
{
public:
    CentralCache()
    {
        for (int i = 0; i < FREELIST_SIZE; i++)
        {
            _freelist[i] = nullptr;
        }
            
    }

    void* FetchRange(size_t slot, size_t n, size_t& actualnum)
    {
        std::lock_guard<std::mutex> lock(mtx);

        void* head = _freelist[slot];
        if (head)
        {
            void *prev = nullptr;
            void *cur = head;
            actualnum = 0;
            while (cur && actualnum < n)
            {
                prev = cur;
                cur = *(void **)cur;
                actualnum++;
            }
            if (prev)
                *(void **)prev = nullptr;
            _freelist[slot] = cur;
            return head;
        }

        size_t obj_size = SlotToSize(slot);
        size_t objs_per_page = 8192 / obj_size;

        size_t num_objs = objs_per_page;
        if (num_objs > MAX_BATCH)
            num_objs = MAX_BATCH;
        if (num_objs < 1)
            num_objs = 1;

        size_t pages = (num_objs * obj_size + 8191) / 8192;
        if (pages < 1) pages = 1;

        span* sp = pagecache.Pushfreelist(pages);
        char* start = (char*)sp->start;
        size_t total_bytes = pages * 8192;
        size_t real_objs = total_bytes / obj_size;

        char* cur2 = start;
        for (size_t i = 0; i < real_objs - 1; i++)
        {
            *(void **)cur2 = cur2 + obj_size;
            cur2 += obj_size;
        }
        *(void **)cur2 = nullptr;

        if (real_objs > n)
        {
            void* result = start;
            char* walk = start;
            for (size_t i = 0; i < n - 1; i++)
            {
                walk += obj_size;
            }
                
            void* rest = walk + obj_size;
            * ( void * * ) walk = nullptr;
            _freelist[slot] = rest;
            actualnum = n;
        }
        else
        {
            _freelist[slot] = nullptr;
            actualnum = real_objs;
        }

        return start;
    }

    void ReleaseRange(size_t slot, void* start, size_t num)
    {
        std::lock_guard<std::mutex> lock(mtx);

        void* cur = start;
        for (size_t i = 0; i < num; i++)
        {
            void *next = *(void **)cur;
            *(void **)cur = _freelist[slot];
            _freelist[slot] = cur;
            cur = next;
        }
    }

private:
    void* _freelist[FREELIST_SIZE];
    std::mutex mtx;
    PageCache pagecache;
};

extern CentralCache centralcache;
