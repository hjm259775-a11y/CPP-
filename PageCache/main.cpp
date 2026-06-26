#include<iostream>
#include"PageCache.h"

int main()
{
    PageCache pc;
    span *s1 = pc.Newspan(1);//要一页
    std::cout << s1->start << " " << s1->size << std::endl;

    span *s2 = pc.Newspan(2);//要两页
    std::cout << s2->start << " " << s2->size << std::endl;

    
    return 0;
}