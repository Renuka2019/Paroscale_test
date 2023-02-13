#include<stdio.h>
#include<stdlib.h>


int main(void)
{
    system(" ps -ef | awk '{print $1,$2}'");
    return 0;
}
