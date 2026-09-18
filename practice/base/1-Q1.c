#include <stdio.h>
#include <time.h>

long Sum1(long n){
    long s = 0;
    for (long i = 1; i <= n; i++)
        s+=i;
    return s;
}

long Sum2(long n){
    long s = n * (n+1) / 2;
    return s;
}

void Show(int n){
    long sum = 0;
    clock_t start,end;

    start = clock();
    for(int i = 1;i <= n;i++)
        sum += Sum1(i);
    end = clock() - start;
    printf("Sum1 = %ld,Sum1 spend %lf\n",sum,((float)end)/CLOCKS_PER_SEC);

    start = clock();
    sum = 0;
    for(int i = 1;i <= n;i++)
        sum += Sum2(i);
    end = clock() - start;
    printf("Sum2 = %ld,Sum2 spend %lf\n",sum,((float)end)/CLOCKS_PER_SEC);
}

int main()
{
    Show(100000);
    return 0;
}