#pragma once
#include"SizeClass.h"
#include"CentralCache.h"

class ThreadCache
{
public:
    void* Allocate(size_t size)
    {
        size_t align_size = roundup(size);
        size_t slot = Index(align_size);

        if (_freelist[slot])
        {
            void* obj = _freelist[slot];
            _freelist[slot] = *(void **)obj;
            return obj;
        }

        size_t actualnum = 0;
        void* batch = centralcache.FetchRange(slot, 8, actualnum);

        if (actualnum == 0)
            return nullptr;

        void* result = batch;
        if (actualnum > 1)
        {
            void *rest = *(void **)batch;
            *(void **)batch = nullptr;
            _freelist[slot] = rest;
        }
        else
        {
            _freelist[slot] = nullptr;
        }

        return result;
    }

    void Deallocate(void* ptr, size_t size)
    {
        size_t align_size = roundup(size);
        size_t slot = Index(align_size);

        *(void **)ptr = _freelist[slot];
        _freelist[slot] = ptr;
        
    }

private:
    void* _freelist[FREELIST_SIZE] = {};
};
