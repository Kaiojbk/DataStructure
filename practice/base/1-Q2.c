#include <stdio.h>
#include <time.h>

long Factorial1(int n){
    long res = 0,tmp = 1;
    for (int i = 1; i <= n; i++){
        tmp = 1;
        for (int j = 1; j <= i; j++)
            tmp *= j;
        res += tmp;
    } 
    return res;
}

long Factorial2(int n){
    long res = 0,tmp = 1;
    for (int i = 1; i <= n; i++){
        tmp *= i;
        res += tmp;
    }
    return res;
}

void Show(int n){
    long sum = 0;
    clock_t start,end;

    start = clock();
    sum = Factorial1(n);
    end = clock() - start;
    printf("Factorial1 = %ld,Factorial1 spend %lf\n",sum,((float)end)/CLOCKS_PER_SEC);

    start = clock();
    sum = Factorial2(n);
    end = clock() - start;
    printf("Factorial1 = %ld,Factorial2 spend %lf\n",sum,((float)end)/CLOCKS_PER_SEC);
}

int main()
{
    Show(15);
    return 0;
}