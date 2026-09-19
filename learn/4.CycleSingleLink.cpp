#include<stdio.h>
#include<malloc.h>

// 循环单向链表的实现(有头节点)

typedef int ElemType;
typedef struct node{
    ElemType data;
    struct node* next;
}SLinkNode;

/// @brief 初始化循环单向链表
/// @param linkList 
void InitList(SLinkNode** linkList){
    *linkList = (SLinkNode*)malloc(sizeof(SLinkNode));
    (*linkList)->next = *linkList;
    printf("初始化链表成功\n");
}

/// @brief 销毁单向链表
/// @param linkList 
void DestroyList(SLinkNode** linkList){
    //防止重复销毁：第一次销毁后已经把指针置空了
    if(linkList==NULL || *linkList==NULL)
        return;
    SLinkNode* currentNode = (*linkList)->next;
    while (currentNode!=*linkList){
        (*linkList)->next=currentNode->next;
        free(currentNode);
        currentNode=(*linkList)->next;
    }
    free(*linkList);
    printf("\n链表销毁完毕\n");
    *linkList=NULL;
}

/// @brief 获取元素个数
/// @param linkList 
/// @return 
int GetLength(SLinkNode* linkList){
    if(linkList==NULL)
        return 0;
    int count = 0;
    SLinkNode* currentNode = linkList->next;
    while (currentNode!=linkList){
        count++;
        currentNode=currentNode->next;
    }
    return count;
}

/// @brief 获取指定位置的元素
/// @param linkList 
/// @param findIndex 
/// @param value 
/// @return 
int GetElem(SLinkNode* linkList,int findIndex,ElemType *value){
    if(linkList==NULL)
        return 0;
    if(findIndex<1)
        return 0;
    SLinkNode* currentNode = linkList->next;
    for (int i = 1; i < findIndex; i++){
        //如果是最后一个节点，直接返回
        if(currentNode==linkList)
            return 0;
        currentNode = currentNode->next;
    }
    //防止空链表
    if(currentNode==linkList)
        return 0;
    *value = currentNode->data;
    return 1;
}

/// @brief 获取第一个值为value的逻辑序号
/// @param linkList 
/// @param value 
/// @return 
int Locate(SLinkNode* linkList,ElemType value){
    //链表不存在时按"找不到"处理，和函数本身的返回值约定保持一致
    if(linkList==NULL)
        return -1;
    int index = 0;
    SLinkNode* tmpNode = linkList->next;
    while(tmpNode!=linkList){
        if(tmpNode->data==value){
            return index+1;
        }
        tmpNode = tmpNode->next;
        index++;
    }
    return -1;
}

/// @brief 在指定位置插入元素
/// @param linkList 
/// @param insPos
/// @param value 
/// @return 
int InsElem(SLinkNode** linkList,int insPos,ElemType value){
    //linkList是被销毁过的指针时，*linkList为NULL，直接拒绝插入
    if(linkList==NULL || *linkList==NULL)
        return 0;
    if(insPos<1)
        return 0;
    SLinkNode* currentNode=*linkList;
    // 先走到插入位置的前一个元素
    for(int i = 0; i < insPos-1; i++){
        currentNode = currentNode->next;
        //先走后判断，如果先判定，那么已进入循环就满足条件了，这里想拦截的是走了一圈后回到头节点的情况
        if(currentNode==*linkList)
            return 0;
    }  
    SLinkNode* insNode = (SLinkNode*)malloc(sizeof(SLinkNode));
    insNode->data = value;
    insNode->next = currentNode->next;
    currentNode->next = insNode;
    return 1;
}

/// @brief 删除指定位置的元素
/// @param linkList 
/// @param delPos 
/// @return 
int DelElem(SLinkNode** linkList,int delPos){
    //linkList是被销毁过的指针时，*linkList为NULL，直接拒绝删除
    if(linkList==NULL || *linkList==NULL)
        return 0;
    if(delPos<1)
        return 0;
    SLinkNode* currentNode = *linkList;
    SLinkNode* delNode;
    // 走到删除节点的前一个节点
    for (int i = 0; i < delPos-1; i++){
        currentNode=currentNode->next;
        if(currentNode==*linkList)
            return 0;
    }
    //经过上一个循环，发现走到了尾节点，说明没有删除的那个位置
    if(currentNode->next==*linkList)
        return 0;
    delNode = currentNode->next;
    currentNode->next = currentNode->next->next;
    free(delNode);
    return 1;
}

/// @brief 遍历输出元素
/// @param linkList 
void PrintList(SLinkNode* linkList){
    if(linkList==NULL){
        printf("链表已经销毁，不能遍历\n");
        return;
    }
    printf("链表元素: ");
    SLinkNode* tmpNode = linkList->next;
    while(tmpNode!=linkList){
        printf("%d ",tmpNode->data);
        tmpNode = tmpNode->next;
    }
    putchar('\n');
}

/// @brief 头插法建表
/// @param linkList 
/// @param data 
/// @param len 
void CreateListF(SLinkNode** linkList,ElemType data[],int len){
    *linkList = (SLinkNode*)malloc(sizeof(SLinkNode));
    (*linkList)->next = *linkList;
    SLinkNode* newNode;
    for (int i = 0; i < len; i++){
        newNode = (SLinkNode*)malloc(sizeof(SLinkNode));
        newNode->data = data[i];
        newNode->next = (*linkList)->next;
        (*linkList)->next = newNode;
    }
}

/// @brief 尾插法建表
/// @param linkList 
/// @param data 
/// @param len 
void CreateListR(SLinkNode** linkList,ElemType data[],int len){
    /*尾插法的预备动作是创建头节点，创建尾节点，
    把尾节点指向头节点，然后开始建表，创建新节点，
    然后尾节点后驱节点指向新节点，
    最后尾节点指向新节点*/
    *linkList = (SLinkNode*)malloc(sizeof(SLinkNode));
    (*linkList)->next = *linkList;
    SLinkNode* newNode;
    SLinkNode* currentNode=*linkList;
    for (int i = 0; i < len; i++){
        newNode = (SLinkNode*)malloc(sizeof(SLinkNode));
        newNode->data = data[i];
        currentNode->next = newNode;
        currentNode = newNode;
    }
    currentNode->next = *linkList;
}


int main()
{
    SLinkNode* list;
    ElemType e;
    ElemType array[] = {1,2,3,4,5};
    InitList(&list);
    InsElem(&list,1,23);
    InsElem(&list,2,24);
    InsElem(&list,3,25);
    InsElem(&list,4,24);
    InsElem(&list,5,26);
    InsElem(&list,6,27);
    InsElem(&list,7,28);
    PrintList(list);
    printf("该链表有%d个元素\n",GetLength(list));
    printf("24是第%d个元素\n",Locate(list,24));
    if(GetElem(list,4,&e))
        printf("第4个元素是%d\n",e);
    printf("删除第8个节点\n");
    DelElem(&list,8);
    PrintList(list);

    DestroyList(&list);
    PrintList(list);

    printf("头插法建表,");
    CreateListF(&list,array,sizeof(array)/sizeof(array[0]));
    PrintList(list);

    DestroyList(&list);

    printf("尾插法建表,");
    CreateListR(&list,array,sizeof(array)/sizeof(array[0]));
    PrintList(list);

    return 0;
}