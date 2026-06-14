#include<stdio.h>
#include<unistd.h>
const char *fengche = "|/-\\";
int main()
{
    char xgz[101] = {0};
    for (int i = 0; i <= 100;)
    {
        xgz[i] = '#';
        printf("\r[%c][%-101s][%3d%%]",fengche[i%4] , xgz, i);
        fflush(stdout);
        i++;
        usleep(100000);
    }
    printf("\n");
    return 0;
}