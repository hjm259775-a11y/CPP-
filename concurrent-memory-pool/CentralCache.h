#include"SizeClass.h"
#include<mutex>
#include<vector>
class CentralCache
{
public:
    void* Popfront(int size,int n)
    {
        void *head = _freelist[n];
        void *end = head;
        for (int i = 0; i < n && end != nullptr; i++)
        {
            end = *(void **)end;
        }

        if(end==nullptr)
        {
            return nullptr;
        }
        _freelist[n] = end;
        return head;
    }

    void Pushfront(int size,int n,void *head)
    {
        
    }

private:
    void* _freelist[208];
    std::mutex mtx;
};
