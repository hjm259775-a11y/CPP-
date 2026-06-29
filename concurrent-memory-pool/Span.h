#pragma once
#include<cstddef>
#include<vector>

struct span
{
    void *start;
    size_t size;
    span *next;
    span *prev;
};


class Spanlist
{
public:
    Spanlist()
    {
        head.next = &head;
        head.prev = &head;
    }

    void Pushspan(span* n)
    {
        n->next = head.next;
        head.next->prev = n;
        head.next = n;
        n->prev = &head;
    }
    span* Popspan()
    {
        if(head.next==&head)
        {
            return nullptr;
        }
        span *h = head.next;
        head.next = head.next->next;
        head.next->prev = &head;
        h->next = nullptr;
        h->prev = nullptr;
        return h;
    }

    bool Emptyspan()
    {
        if(head.prev==&head || head.next==&head)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
private:
    span head;
};