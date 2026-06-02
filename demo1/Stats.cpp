#include"Stats.h"
#include<vector>
using namespace std;


    int caozuo::sum(const vector<int> &a)
    {
        int ans=0;
        for(int i=0;i<a.size();i++)
        {
            ans+=a[i];
        }
        return ans;
    }

    double caozuo::average(const vector<int> &a)
    {
        return double(sum(a))/a.size();
    }

    int caozuo::maxx(const vector<int> &a)
    {
        int hh=a[0];
        for(int i=0;i<a.size();i++)
        {
            if(a[i]>hh)
            {
                hh=a[i];
            }
        }
        return hh;
    }
