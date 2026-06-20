#include<mutex>




class CentralCache
{
public:
    CentralCache()
    {
        for (int i = 0; i < 200;i++)
        {
            _freelist[0] = nullptr;
        }
    }

    void* FetchRange(int size,int num,int actualnum)
    {
        
    }
    void* ReleaseRange(void* start,size_t size,size_t num)
    {

    }



private:
    void *_freelist[200];

};