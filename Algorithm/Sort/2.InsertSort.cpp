#include<stdio.h>
#include"GenerateNums.cpp"
#define MaxSize 10

void InsertSort(int arr[],int len){

}

int main()
{
    int nums[MaxSize];
    Generate(nums,MaxSize);
    printf("未排序数组>>>");
    PrintNums(nums,MaxSize);
    InsertSort(nums,MaxSize);
    return 0;
}