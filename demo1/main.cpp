#include "Stats.h"
#include <iostream>
using namespace std;
int main()
{
    vector<int> xgz = {5, 25, 14, 26, 15, 36};
    for (auto v : xgz)
    {
        cout << v << " ";
    }
    cout << endl;

    cout << "和：" << caozuo::sum(xgz) << endl;

    cout << "平均值：" << caozuo::average(xgz) << endl;

    cout << "最大值：" << caozuo::maxx(xgz) << endl;

    return 0;
}