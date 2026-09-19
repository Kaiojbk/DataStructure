#include<stdio.h>
#define MaxSize 100

// 顺序表的实现

typedef int ElemType;
typedef struct{
    ElemType data[MaxSize];
    int length;
}SqList;

// 初始化
void InitList(SqList *l){
    l->length = 0;
}

// 销毁
void DestroyList(SqList l){
    //  动态分配的，不需要做什么操作
}

// 获取表长
int GetLength(SqList l){
    return l.length;
}

// 按序查找
int GetElem(SqList l,int index,ElemType* data){
    if(index<0 || index>l.length)
        return 0;
    *data = l.data[index-1];
    return 1;
}

// 按值查找
int Locate(SqList l,ElemType data){
    for (int i = 0; i < l.length; i++){
        if(l.data[i]==data)
            return i+1;
    }
    return 0;
}

// 插入元素
int InsertElem(SqList *l,ElemType data,int insertPos){
    if(insertPos<1 || insertPos>l->length+1 || l->length==MaxSize)
        return 0;
    for (int i = l->length; i > insertPos -1; i--)
        l->data[i] = l->data[i-1];
    l->data[insertPos-1] = data;
    l->length++;
    return 1;
}

// 删除元素
int DeleteElem(SqList *l,int deletePos){
    if(deletePos<=0 || deletePos>=l->length)
        return 0;
    for (int i = deletePos - 1; i < l->length; i++)
        l->data[i] = l->data[i+1];
    l->length-=1;
    return 1;
}

// 遍历显示元素
void Display(SqList l){
    for (int i = 0; i < l.length; i++)
        printf("%d ",l.data[i]);
}

int main()
{
    int i;
    ElemType e;
    SqList list;
    InitList(&list);
    InsertElem(&list,1,1);
    InsertElem(&list,3,2);
    InsertElem(&list,1,3);
    InsertElem(&list,5,4);
    InsertElem(&list,4,5);
    InsertElem(&list,2,6);
    Display(list);
    printf("数组长度%d\n",GetLength(list));
    i=3;
    e=4;
    if(GetElem(list,i,&e))
        printf("第%d个元素是%d\n",i,e);
    printf("%d是第%d个元素\n",e,Locate(list,e));
    printf("删除第%d个元素后\n",i);
    DeleteElem(&list,i);
    Display(list);
    DestroyList(list);
    return 0;
}