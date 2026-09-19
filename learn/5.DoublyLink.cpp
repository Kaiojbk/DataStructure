#include<stdio.h>
#include<malloc.h>

// 双向链表的实现(带头节点，非循环)
// 约定：头节点不存数据，空表时头节点的 pre 和 next 都为 NULL
// 本篇行参统一用 *& (指针的引用)：函数要改的就是"调用方那个指针本身"，
// 调用时直接写 list，不用像前面几篇那样写 &list

typedef int ElemType;
typedef struct node{
    ElemType data;
    struct node* pre;
    struct node* next;
}DoublyLinkNode;

/// @brief 初始化双向链表
/// @param dlk
void InitList(DoublyLinkNode*& dlk){
    // TODO: 申请头节点，把 pre、next 都置空
    dlk = (DoublyLinkNode*)malloc(sizeof(DoublyLinkNode));
    dlk->next = NULL;
    dlk->pre = NULL;
}

/// @brief 销毁双向链表
/// @param dlk
void DestroyList(DoublyLinkNode*& dlk){
    // TODO: 逐个释放节点，最后把 dlk 置空（防止重复销毁）
    if(dlk==NULL)
        return;
    DoublyLinkNode* ptNode = dlk;
    DoublyLinkNode* freeNode;
    while(ptNode!=NULL){
        freeNode = ptNode->next;
        free(freeNode);
        ptNode=ptNode->next;
    }
    free(dlk);
    dlk=NULL;
}

/// @brief 获取元素个数（不含头节点）
/// @param dlk
/// @return
int GetLength(DoublyLinkNode* dlk){
    if(dlk==NULL)
        return 0;
    int count = 0;
    while(dlk->next!=NULL){
        count++;
        dlk = dlk->next;
    }
    return count;
}

/// @brief 获取指定位置的元素（位置从 1 开始）
/// @param dlk
/// @param findIndex
/// @param value
/// @return 成功返回 1，失败(越界/空表)返回 0
int GetElem(DoublyLinkNode* dlk,int findIndex,ElemType *value){
    if(dlk==NULL)
        return 0;
    DoublyLinkNode* ptNode = dlk;
    for (int i = 0; i < findIndex-1; i++){
        if(ptNode->next==NULL)
            return 0;
        ptNode = ptNode->next;
    }
    *value = ptNode->next->data;
    return 1;
}

/// @brief 获取第一个值为 value 的逻辑序号
/// @param dlk
/// @param value
/// @return 找到返回序号，找不到返回 -1
int Locate(DoublyLinkNode* dlk,ElemType value){
    if(dlk==NULL)
        return -1;
    int index = 0;
    while(dlk!=NULL){
        if(dlk->data==value)
            return index;
        dlk=dlk->next;
        index++;
    }
    return -1;
}

/// @brief 在指定位置插入元素
/// @param dlk
/// @param insPos 取值范围 [1, 长度+1]
/// @param value
/// @return 成功返回 1，失败返回 0
int InsElem(DoublyLinkNode*& dlk,int insPos,ElemType value){
    // TODO: 找到插入位置的前一个节点，新节点要接两条线(pre 和 next)，
    //       别忘了新节点后面那个节点的 pre 也要改（一共 4 个指针）

    // 禁止非法插入
    if(dlk==NULL||insPos<1)
        return 0;
    DoublyLinkNode* newNode;
    DoublyLinkNode* ptNode=dlk;
    for (int i = 0; i < insPos-1; i++){
        if(ptNode==NULL)
            return 0;
        ptNode=ptNode->next;
    }
    if(ptNode==NULL)
        return 0;
    newNode = (DoublyLinkNode*)malloc(sizeof(DoublyLinkNode));
    newNode->data = value;
    newNode->next = ptNode->next;
    newNode->pre = ptNode;
    //当且仅当不是插入到最后一个元素的时候才执行的操作
    if(ptNode->next!=NULL)
        ptNode->next->pre = newNode;
    ptNode->next = newNode;
    return 1;
}

/// @brief 删除指定位置的元素
/// @param dlk
/// @param delPos
/// @return 成功返回 1，失败返回 0
int DelElem(DoublyLinkNode*& dlk,int delPos){
    // TODO: 同样是改两条线，注意被删节点是最后一个时，它后面没有节点，
    //       不能直接写 delNode->next->pre = ptNode;
    if(dlk==NULL||delPos<1)
        return 0;
    DoublyLinkNode* delNode;
    DoublyLinkNode* ptNode=dlk;
    for (int i = 0; i < delPos-1; i++){
        if(ptNode->next==NULL)
            return 0;
        ptNode=ptNode->next;
    }
    delNode = ptNode->next;
    //当且仅当删除元素不是最后一个元素的时候才执行的操作
    if(delNode->next!=NULL)
        delNode->next->pre = ptNode;
    ptNode->next = delNode->next;
    free(delNode);
    return 1;
}

/// @brief 正向遍历输出元素
/// @param dlk
void PrintList(DoublyLinkNode* dlk){
    if(dlk==NULL){
        printf("链表为空不可遍历\n");
        return;
    }
    printf("正向遍历：");
    DoublyLinkNode* ptNode = dlk->next;
    while(ptNode!=NULL){
        printf("%d ",ptNode->data);
        ptNode=ptNode->next;
    }
    putchar('\n');
}

/// @brief 反向遍历输出元素（双向链表特有的，从尾节点沿 pre 往回走）
/// @param dlk
void PrintListReverse(DoublyLinkNode* dlk){
    if(dlk==NULL){
        printf("链表为空不可遍历\n");
        return;
    }
    printf("反向遍历：");
    DoublyLinkNode* ptNode = dlk->next;
    while(ptNode!=NULL){
        if(ptNode->next==NULL)
            break;
        ptNode=ptNode->next;
    }
    while(ptNode!=dlk){
        printf("%d ",ptNode->data);
        ptNode=ptNode->pre;
    }
    putchar('\n');
}

/// @brief 头插法建表
/// @param dlk
/// @param data
/// @param len
void CreateListF(DoublyLinkNode*& dlk,ElemType data[],int len){
    dlk = (DoublyLinkNode*)malloc(sizeof(DoublyLinkNode));
    //  一定要tm初始化啊！！！
    dlk->next = dlk->pre = NULL;
    DoublyLinkNode* newNode;
    for (int i = 0; i < len; i++){
        newNode = (DoublyLinkNode*)malloc(sizeof(DoublyLinkNode));
        newNode->data = data[i];
        newNode->next = dlk->next;
        newNode->pre = dlk;
        if(dlk->next!=NULL)
            dlk->next->pre = newNode;
        dlk->next = newNode;
    }
}

/// @brief 尾插法建表
/// @param dlk
/// @param data
/// @param len
void CreateListR(DoublyLinkNode*& dlk,ElemType data[],int len){
    dlk = (DoublyLinkNode*)malloc(sizeof(DoublyLinkNode));
    DoublyLinkNode* newNode;
    DoublyLinkNode* ptrNode = dlk;
    for (int i = 0; i < len; i++){
        newNode = (DoublyLinkNode*)malloc(sizeof(DoublyLinkNode));
        newNode->data = data[i];
        ptrNode->next = newNode;
        newNode->pre = ptrNode;
        ptrNode = newNode;
    }
    ptrNode->next = NULL;
}


int main()
{
    DoublyLinkNode* list = NULL;
    ElemType e;
    ElemType array[] = {1,2,3,4,5};
    int len = sizeof(array)/sizeof(array[0]);

    printf("=== 1. 初始化 ===\n");
    InitList(list);
    PrintList(list);                        // 期望：空链表
    printf("长度: %d\n",GetLength(list));    // 期望：0

    printf("\n=== 2. 插入 ===\n");
    printf("空表插第1个: %d\n",InsElem(list,1,10));
    printf("插到第2个: %d\n",InsElem(list,2,20));
    printf("插到第1个(头插): %d\n",InsElem(list,1,5));
    printf("插到第4个(尾插): %d\n",InsElem(list,4,30));
    PrintList(list);                        // 期望：5 10 20 30
    PrintListReverse(list);                 // 期望：30 20 10 5（反向验证 pre 接对了）
    printf("长度: %d\n",GetLength(list));    // 期望：4
    printf("越界插入(位置0): %d\n",InsElem(list,0,99));    // 期望：0
    printf("越界插入(位置100): %d\n",InsElem(list,100,99));// 期望：0

    printf("\n=== 3. 查找 ===\n");
    printf("20的位置: %d\n",Locate(list,20));        // 期望：3
    printf("不存在元素88的位置: %d\n",Locate(list,88));// 期望：-1
    if(GetElem(list,2,&e))
        printf("第2个元素: %d\n",e);                 // 期望：10
    printf("取越界位置(100): %d\n",GetElem(list,100,&e));// 期望：0

    printf("\n=== 4. 删除 ===\n");
    printf("删除第1个(删头): %d\n",DelElem(list,1));
    PrintList(list);                        // 期望：10 20 30
    printf("删除最后一个: %d\n",DelElem(list,GetLength(list)));
    PrintList(list);                        // 期望：10 20
    printf("越界删除(位置100): %d\n",DelElem(list,100)); // 期望：0

    printf("\n=== 5. 销毁 ===\n");
    DestroyList(list);
    PrintList(list);                        // 期望：提示链表已销毁
    printf("销毁后长度: %d\n",GetLength(list));// 期望：0

    printf("\n=== 6. 头插法建表 ===\n");
    CreateListF(list,array,len);
    PrintList(list);                        // 期望：5 4 3 2 1
    DestroyList(list);

    printf("\n=== 7. 尾插法建表 ===\n");
    CreateListR(list,array,len);
    PrintList(list);                        // 期望：1 2 3 4 5
    PrintListReverse(list);                 // 期望：5 4 3 2 1
    DestroyList(list);

    return 0;
}
