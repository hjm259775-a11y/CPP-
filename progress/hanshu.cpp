#include"tou.h"

const std::string xgz = "|/-\\";
char jindu[NUM];

void qingkong()
{
    memset(jindu, '\0', sizeof(jindu));
}

void xianshi(void (*zhege)(int))
{
    int quan = 1000;
    int cur = 0;
    while(cur<=quan)
    {
        usleep(50000);
        int per = (cur  * 100 / quan);
        zhege(per);
        cur += 10;
    }
    printf("\n");
}


void moni(int per)
{
    if(per<0 ||per>baifenbi)
    {
        printf("错误：%d%%", per);
        return;
    }
    int longg = xgz.size();
    jindu[per] = zifu;
    if(per!=100)
    {
        int hhh = per + 1;
        jindu[hhh] = jiantou;
    }
    printf("\r[%c][%-101s][%-3d%%]", xgz[per % longg], jindu, per);
    fflush(stdout);


}