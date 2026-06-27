#include<iostream>
#include"PageCache.h"
#include<vector>
#include<thread>

PageCache pc1;

void worker(int id,std::vector<span*>& spans,std::mutex& collect_mtx)
{
    for (int i = 0; i < 3;i++)
    {
        span *h = pc1.Newspan(id + i);

        std::lock_guard<std::mutex> lock(collect_mtx);
        spans.push_back(h);
    }
}

int main()
{
    PageCache pc;
    span *s1 = pc.Newspan(1);//要一页
    std::cout << s1->start << " " << s1->size << std::endl;

    span *s2 = pc.Newspan(2);//要两页
    std::cout << s2->start << " " << s2->size << std::endl;

    pc.DeleteSpan(s2);//删了s2
    span *s3 = pc.Newspan(2);
    std::cout << s3->start << " " << s3->size << std::endl;//复用了s2的地址

    std::cout << std::endl;

    span *a = pc.Newspan(1);
    span *b = pc.Newspan(3);
    span *c = pc.Newspan(5);
    std::cout << a->start << "<a<" << (void*)((char *)a->start + a->size * 8192) << std::endl;
    std::cout << b->start << "<b<" << (void*)((char *)b->start + b->size * 8192) << std::endl;
    std::cout << c->start << "<c<" << (void*)((char *)c->start + c->size * 8192) << std::endl;




    //多线程测试
    std::vector<span *> spans;
    std::mutex collect_mtx;

    std::thread t0(worker, 0, std::ref(spans), std::ref(collect_mtx));
    std::thread t1(worker, 1, std::ref(spans), std::ref(collect_mtx));
    std::thread t2(worker, 2, std::ref(spans), std::ref(collect_mtx));
    std::thread t3(worker, 3, std::ref(spans), std::ref(collect_mtx));

    t0.join();
    t1.join();
    t2.join();
    t3.join();

    for (int i = 0; i < spans.size();i++)
    {
        char *i_start = (char *)(spans[i]->start);
        char *i_end = (char *)(spans[i]->start) + (spans[i]->size) * 8192;
        for (int j = i + 1; j < spans.size();j++)
        {
            char *j_start = (char *)(spans[j]->start);
            char *j_end = (char *)(spans[j]->start) + (spans[j]->size) * 8192;
            if(!(i_end<=j_start||j_end<=i_start))
            {
                std::cout << "错误" << std::endl;
            }
        }
    }
    std::cout << std::endl;
    std::cout << spans.size() << std::endl;

    
    return 0;
}