#pragma once
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
    else
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
    else
    {
        return 16 + 56 + (n + 31) / 32;
    }
}

// slot → 对齐后的大小（与 Index 互逆）
size_t SlotToSize(size_t slot)
{
    if(slot <= 16)     return slot * 8;                    // 8,16,...,128
    if(slot <= 80)     return 128 + (slot - 24) * 16;      // 144,160,...,1024
    /* <= 200 */       return 1024 + (slot - 104) * 32;    // 1056,1088,...,4096
}
