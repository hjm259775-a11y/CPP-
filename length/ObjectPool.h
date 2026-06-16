#pragma once
#include<vector>
#include<cstdlib>
#include<cstddef>

const size_t BLOCK_SIZE = 64 * 1024;

template <class T>
class ObjectPool
{
public:
    ObjectPool()
    {
        _freelist = nullptr;
        block_r = 0;
        _current = nullptr;
    }
    ~ObjectPool()
    {
        for(auto block:_blocks)
        {
            std::free(block);
        }
        _blocks.clear();
    }

    T* New()
    {
        if(_freelist!=nullptr)
        {
            void *obj = _freelist;
            _freelist = *(void **)_freelist;
            return (T *)obj;
        }
        if(block_r<sizeof(T))
        {
            void *newblock = std::malloc(BLOCK_SIZE);
            _blocks.push_back(newblock);
            _current = (char *)newblock;
            block_r = BLOCK_SIZE;
        }
        void *obj = _current;
        _current += sizeof(T);
        block_r -= sizeof(T);
        return (T *)obj;
    }

    void Delete(T* obj)
    {
        *(void **)obj = _freelist;
        _freelist = obj;
    }

private:
    void* _freelist;//链表头
    std::vector<void *> _blocks;//所有内存块
    size_t block_r;//内存块剩余内存
    char *_current;//当前块下一个可用位置
};