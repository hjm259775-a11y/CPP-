#include"ObjectPool.h"
#include<iostream>
#include<chrono>

const long long N = 100000000;
int main()
{
    ObjectPool<double> xgz1;
    double *a = xgz1.New();
    double *b = xgz1.New();
    *a = 42;
    *b = 100;
    std::cout << "a:" << *a << " " << "b:" << *b << std::endl;
    xgz1.Delete(a);
    xgz1.Delete(b);
    double *c = xgz1.New();
    std::cout << "c:" << *c << std::endl;//会输出b的地址，被复用了
    //快，但不具备初始化

    ObjectPool<double> xgz;
    std::vector<double *> myn(N);



    auto time1 = std::chrono::high_resolution_clock::now(); // 记录当前时间
    for (int i = 0; i < N;i++)
    {
        myn[i]=xgz.New();
    }
    for (int i = 0; i < N;i++)
    {
        xgz.Delete(myn[i]);
    }
    auto time2 = std::chrono::high_resolution_clock::now();



    auto time3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N;i++)
    {
        myn[i] = new double;
    }
    for (int i = 0; i < N;i++)
    {
        delete myn[i];
    }
    auto time4 = std::chrono::high_resolution_clock::now();

    auto pool = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    auto ll = std::chrono::duration_cast<std::chrono::milliseconds>(time4 - time3).count();

    std::cout << pool << std::endl;
    std::cout << ll << std::endl;
    return 0;
}