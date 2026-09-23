#include<stdio.h>
#include"GenerateNums.cpp"
#define MaxSize 10

void BubbleSort(int arr[],int len){
    if(len<=0)
        return;
    for (int i = 0; i < len; i++){
        for (int j = 0; j < len-1-i; j++){
            if(arr[j]>arr[j+1]){
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
        printf("第%d次冒泡排序>>>",i+1);
        PrintNums(arr,len);
    }
}

int main()
{
    int nums[MaxSize];
    Generate(nums,MaxSize);
    printf("未排序数组>>>");
    PrintNums(nums,MaxSize);
    BubbleSort(nums,MaxSize);
    return 0;
}