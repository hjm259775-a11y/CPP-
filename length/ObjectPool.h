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
            delete(block);
        }
        _blocks.clear();
    }

    T* New()
    {
        if(_freelist!=nullptr)
        {
            
        }
    }

    void Delete()
    {

    }

private:
    void* _freelist;//链表头
    std::vector<void *> _blocks;//所有内存块
    size_t block_r;//内存块剩余内存
    char *_current;//当前块下一个可用位置
};