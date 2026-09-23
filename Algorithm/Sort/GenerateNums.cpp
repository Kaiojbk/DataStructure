#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/// @brief 随机生成len个数字再arr中，范围0~99
/// @param arr 
/// @param len 
void Generate(int arr[],int len){
    if(len<1)
        return;
    srand((unsigned int)time(NULL));
    for (int  i = 0; i < len; i++){
        arr[i] = rand()%100;
    }
}

void PrintNums(int arr[],int len){
    if(len<1)
        return;
    for (int  i = 0; i < len; i++)
        printf("%d ",arr[i]);
    putchar('\n');
}

