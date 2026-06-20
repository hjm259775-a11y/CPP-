#include<cstddef>



size_t RoundUp(size_t n)
{
    if(n<=128)
    {
        return ((n + 7) / 8) * 8;
    }
    else if(n<=1024)
    {
        return ((n + 15) / 16) * 16;
    }
    else if(n<=4096)
    {
        return ((n + 31) / 32) * 32;
    }
}




size_t Index(size_t n)
{
    if(n<=128)
    {
        return (n + 7) / 8;
    }
    else if(n<=1024)
    {
        return 16 + (n + 15) / 16;
    }
    else if(n<=4096)
    {
        return 16 + 56 + (n + 31) / 32;
    }
}