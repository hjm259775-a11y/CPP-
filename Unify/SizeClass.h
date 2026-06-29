#pragma once
#include<cstddef>
#include<cassert>

size_t roundup(size_t t)
{
    if(t<=128)
    {
        return ((t + 7) / 8)*8;
    }
    else if(t<=1024)
    {
        return ((t + 15) / 16) * 16;
    }
    else if(t<=8*1024)
    {
        return ((t + 127) / 128)*128;
    }
    else if(t<=64*1024)
    {
        return ((t + 1023) / 1024)*1024;
    }
    else{
        return ((t + (8 * 1024 - 1)) / (8 * 1024)) * (8 * 1024);
    }
}


size_t Index(size_t n)
{
    assert(n > 0);
    if (n <= 128)
    {
        return (n + 7) / 8;
    }
    else if(n<=1024)
    {
        return 16 + (n - 128 + 15) / 16;
    }
    else if(n<=8*1024)
    {
        return 16 + 56 + (n - 1024 + 127) / 128;
    }
    else if(n<=64*1024)
    {
        return 16 + 56 + 56 + (n - 8 * 1024 + 1023) / 1024;
    }
    else
    {
        return 16 + 56 + 56 + 56 + (n - 64 * 1024 + (8 * 1024 - 1)) / (8 * 1024);
    }
}
//上限是256KB


size_t SlotToSize(size_t slot)
{
    if(slot <= 16)     return slot * 8;
    if(slot <= 72)     return 128 + (slot - 16) * 16;
    if(slot <= 128)    return 1024 + (slot - 72) * 128;
    if(slot <= 184)    return 8 * 1024 + (slot - 128) * 1024;
    /* <= 208 */       return 64 * 1024 + (slot - 184) * 8 * 1024;
}
