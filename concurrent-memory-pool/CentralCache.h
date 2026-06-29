#include"SizeClass.h"
#include<mutex>
#include<vector>
class CentralCache
{
public:
    
private:
    void* _freelist[208];
    std::mutex mtx;
};
