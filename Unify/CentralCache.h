#pragma once
#include<mutex>
#include"SizeClass.h"
#include"PageCache.h"

const size_t MAX_BATCH = 32; // 每次从PageCache拿一批对象不超过32个

class CentralCache
{
public:
    static CentralCache& GetInstance()
    {
        static CentralCache inst;
        return inst;
    }

    // 从 slot 拿 n 个对象返回给 ThreadCache，actualnum 返回实际拿到多少
    void* FetchRange(size_t slot, size_t n, size_t& actualnum)
    {
        std::lock_guard<std::mutex> lock(mtx);

        // 先看看 freelist 里还有没有
        void* head = _freelist[slot];
        if (head)
        {
            void* prev = nullptr;
            void* cur = head;
            actualnum = 0;
            while (cur && actualnum < n)
            {
                prev = cur;
                cur = * ( void * * ) cur;
                actualnum++;
            }
            // 关键修复：断开第 n 个对象和第 n+1 个对象的链接
            // 防止 ThreadCache 顺着链走到 CentralCache 保留的对象上
            if (prev)
                * ( void * * ) prev = nullptr;
            _freelist[slot] = cur; // 剩下的还留在 central cache
            return head;
        }

        // slot 空了 → 从 PageCache 拿 span，切成对象
        size_t obj_size = SlotToSize(slot);
        size_t objs_per_page = 8192 / obj_size;

        // 每次至少拿 1 页，最多 MAX_BATCH 个对象
        size_t num_objs = objs_per_page;
        if (num_objs > MAX_BATCH)
            num_objs = MAX_BATCH;
        if (num_objs < 1)
            num_objs = 1;

        size_t pages = (num_objs * obj_size + 8191) / 8192;
        if (pages < 1) pages = 1;

        span* sp = PageCache::GetInstance().Newspan(pages);
        char* start = (char*)sp->start;
        size_t total_bytes = pages * 8192;
        size_t real_objs = total_bytes / obj_size;

        // 用嵌入式指针把所有对象串成链表
        char* cur = start;
        for (size_t i = 0; i < real_objs - 1; i++)
        {
            * ( void * * ) cur = cur + obj_size;
            cur += obj_size;
        }
        * ( void * * ) cur = nullptr;  // 最后一个指向空

        // 返回前 n 个，剩下的放回 _freelist[slot]
        if (real_objs > n)
        {
            void* result = start;
            char* walk = start;
            for (size_t i = 0; i < n - 1; i++)
                walk += obj_size;
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

    // 把一串对象归还到 slot
    void ReleaseRange(size_t slot, void* start, size_t num)
    {
        std::lock_guard<std::mutex> lock(mtx);

        void* cur = start;
        for (size_t i = 0; i < num; i++)
        {
            void* next = * ( void * * ) cur;
            * ( void * * ) cur = _freelist[slot];
            _freelist[slot] = cur;
            cur = next;
        }
    }

private:
    CentralCache()
    {
        for (int i = 0; i < 200; i++)
            _freelist[i] = nullptr;
    }
    CentralCache(const CentralCache&) = delete;
    CentralCache& operator=(const CentralCache&) = delete;

    void* _freelist[200];
    std::mutex mtx;
};
