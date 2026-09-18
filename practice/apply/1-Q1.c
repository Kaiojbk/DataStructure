#include<stdio.h>
#define N 100

void FindMax(int arr[],int len){
    int first_max = arr[0];
    int second_max = arr[0];
    for (int i = 1; i < len; i++)
    {
        if(arr[i]>=first_max){
            second_max = first_max;
            first_max = arr[i];
        }
    }
    printf("first_max=%d,second_max=%d ",first_max,second_max);
}

void Input(int* a,int* len){
    char ch;
    int i = 0;
    do{
        scanf("%d",&a[i++]);
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
