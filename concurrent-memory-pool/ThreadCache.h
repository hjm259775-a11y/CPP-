#pragma once
#include"CentralCache.h"


extern CentralCache centralcache;//所有线程指向的唯一CentralCache，不能写在类内，类内即为

class ThreadCache
{
public:
    ThreadCache()
    {
        for (int i = 0; i < 208;i++)
        {
            _freelist_[i] = nullptr;
        }
    }
    void* New(size_t k)
    {
        size_t shiji = Index(k);
        if (_freelist_[shiji]==nullptr)
        {
            void *stl = centralcache.Popfront(k, 1);
            return stl;
        }
        else
        {
            void *stl = _freelist_[shiji];
            _freelist_[shiji] = *(void **)stl;
            return stl;
        }
    }

    void Delete(void* w,size_t k)
    {
        size_t shiji = Index(k);
        *(void **)w = _freelist_[shiji];
        _freelist_[shiji] = w;
    }

private:
    void *_freelist_[208];
};