#include"ObjectPool.h"
#include<iostream>

const long long N = 100000000;
int main()
{
    ObjectPool<double> xgz;
    double *a = xgz.New();
    double *b = xgz.New();
    *a = 42;
    *b = 100;
    std::cout << "a:" << *a << " " << "b:" << *b << std::endl;
    xgz.Delete(a);
    xgz.Delete(b);
    double *c = xgz.New();
    std::cout << "c:" << *c << std::endl;//会输出b的地址，被复用了
    return 0;
}