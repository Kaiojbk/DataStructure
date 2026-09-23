#include<stdio.h>
#define N 100

void FindMax(int arr[],int len){
    if(len<=0)
        return;
    if(len==1){
        printf("first_max=%d,second_max=%d ",arr[0],arr[0]);
        return;
    }
    int first_max,second_max;
    if(arr[0]>arr[1]){
        first_max = arr[0];
        second_max = arr[1];
    }
    else{
        first_max = arr[1];
        second_max = arr[0];
    }
    for (int i = 2; i < len; i++)
    {
        if(arr[i]>=first_max){
            second_max = first_max;
            first_max = arr[i];
        }
        if(arr[i]>=second_max){
            second_max = arr[i];
        }
    }
    printf("first_max=%d,second_max=%d ",first_max,second_max);
}

void Input(int* a,int* len){
    char ch;
    int i = 0;
    do{
        if(scanf("%d",&a[i++])!=1)
            break;
    } while((ch = getchar()) != '\n');
    *len = i;
}

int main()
{
    int array[N];
    int len;
    Input(array,&len);
    FindMax(array,len);
    
    return 0;
}
