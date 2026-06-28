#pragma once
#include<cstddef>

struct span
{
    void *start;
    size_t size;
    bool _is;
    span *next;
    span *prev;
};

class SpanList
{
public:
    SpanList()
    {
        head.next = &head;
        head.prev = &head;
    }

    void pushfront(span* n)
    {
        n->next = head.next;
        head.next->prev = n;
        head.next = n;
        n->prev = &head;
    }

    span* popfront()
    {
        span *der = head.next;
        span *derr = der->next;
        derr->prev = &head;
        head.next = derr;
        return der;
    }

    void* remove(span* n)
    {
        span *r = n->next;
        span *l = n->prev;
        l->next = r;
        r->prev = l;
        n->next = nullptr;
        n->prev = nullptr;

        void *h = n->start;
        return h;
    }

    bool Empty()
    {
        return head.next == &head && head.prev == &head;
    }

private:
    span head;
};
