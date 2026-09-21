#include<stdio.h>
#include<malloc.h>

// 循环双向链表的实现(带头节点)
// 约定：头节点不存数据，空表时头节点的 pre 和 next 都为 NULL
// 本篇行参统一用 *& (指针的引用)：函数要改的就是"调用方那个指针本身"，
// 调用时直接写 list，不用像前面几篇那样写 &list

typedef int ElemType;
typedef struct node{
    ElemType data;
    struct node* pre;
    struct node* next;
}CycleDoublyLinkNode;

/// @brief 初始化双向链表
/// @param dlk
void InitList(CycleDoublyLinkNode*& dlk){
    // TODO: 申请头节点，把 pre、next 都指向头结点
    dlk = (CycleDoublyLinkNode*)malloc(sizeof(CycleDoublyLinkNode));
    dlk->next = dlk->pre = dlk;
}

/// @brief 销毁双向链表
/// @param dlk
void DestroyList(CycleDoublyLinkNode*& dlk){
    // TODO: 逐个释放节点，最后把 dlk 置空（防止重复销毁）
    if(dlk==NULL)
        return;
    CycleDoublyLinkNode* ptNode = dlk->next;
    CycleDoublyLinkNode* freeNode;
    while(ptNode!=dlk){
        freeNode = ptNode;
        ptNode=ptNode->next;
        free(freeNode);
    }
    free(dlk);
    dlk=NULL;
}

/// @brief 获取元素个数（不含头节点）
/// @param dlk
/// @return
int GetLength(CycleDoublyLinkNode* dlk){
    if(dlk==NULL)
        return 0;
    int count = 0;
    CycleDoublyLinkNode* ptNode = dlk->next;
    while(ptNode!=dlk){
        count++;
        ptNode = ptNode->next;
    }
    return count;
}

/// @brief 获取指定位置的元素（位置从 1 开始）
/// @param dlk
/// @param findIndex
/// @param value
/// @return 成功返回 1，失败(越界/空表)返回 0
int GetElem(CycleDoublyLinkNode* dlk,int findIndex,ElemType *value){
    if(dlk==NULL||findIndex<1||findIndex>GetLength(dlk))
        return 0;
    CycleDoublyLinkNode* ptNode = dlk;
    for (int i = 0; i < findIndex-1; i++){
        if(ptNode->next==dlk)
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
int Locate(CycleDoublyLinkNode* dlk,ElemType value){
    if(dlk==NULL)
        return -1;
    int index = 0;
    CycleDoublyLinkNode* ptNode = dlk->next;
    while(ptNode!=dlk){
        if(ptNode->data==value)
            return ++index;     // 比对成功，但是索引还没来得及+1
        ptNode=ptNode->next;
        index++;
    }
    return -1;
}

/// @brief 在指定位置插入元素
/// @param dlk
/// @param insPos 取值范围 [1, 长度+1]
/// @param value
/// @return 成功返回 1，失败返回 0
int InsElem(CycleDoublyLinkNode*& dlk,int insPos,ElemType value){
    // 禁止非法插入
    if(dlk==NULL||insPos<1)
        return 0;
    CycleDoublyLinkNode* newNode;
    CycleDoublyLinkNode* ptNode=dlk;
    for (int i = 0; i < insPos-1; i++){
        ptNode=ptNode->next;
        if(ptNode==dlk)
            return 0;
    }
    newNode = (CycleDoublyLinkNode*)malloc(sizeof(CycleDoublyLinkNode));
    newNode->data = value;
    newNode->next = ptNode->next;
    newNode->pre = ptNode;
    ptNode->next->pre = newNode;
    ptNode->next = newNode;
    return 1;
}

/// @brief 删除指定位置的元素
/// @param dlk
/// @param delPos
/// @return 成功返回 1，失败返回 0
int DelElem(CycleDoublyLinkNode*& dlk,int delPos){
    if(dlk==NULL||delPos<1||GetLength(dlk)<delPos)
        return 0;
    CycleDoublyLinkNode* delNode;
    CycleDoublyLinkNode* ptNode=dlk;
    for (int i = 0; i < delPos-1; i++){
        ptNode=ptNode->next;
        if(ptNode==dlk)
            return 0;
    }
    delNode = ptNode->next;
    delNode->next->pre = ptNode;
    ptNode->next = delNode->next;
    free(delNode);
    return 1;
}

/// @brief 正向遍历输出元素
/// @param dlk
void PrintList(CycleDoublyLinkNode* dlk){
    if(dlk==NULL){
        printf("链表为空不可遍历\n");
        return;
    }
    printf("正向遍历：");
    CycleDoublyLinkNode* ptNode = dlk->next;
    while(ptNode!=dlk){
        printf("%d ",ptNode->data);
        ptNode=ptNode->next;
    }
    putchar('\n');
}

/// @brief 反向遍历输出元素（双向链表特有的，从尾节点沿 pre 往回走）
/// @param dlk
void PrintListReverse(CycleDoublyLinkNode* dlk){
    if(dlk==NULL){
        printf("链表为空不可遍历\n");
        return;
    }
    printf("反向遍历：");
    CycleDoublyLinkNode* ptNode = dlk->next;
    while(ptNode!=dlk){
        if(ptNode->next==dlk)
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
void CreateListF(CycleDoublyLinkNode*& dlk,ElemType data[],int len){
    if(len<=0)
        return;
    dlk = (CycleDoublyLinkNode*)malloc(sizeof(CycleDoublyLinkNode));
    dlk->next = dlk->pre = dlk;
    CycleDoublyLinkNode* newNode;
    for (int i = 0; i < len; i++){
        newNode = (CycleDoublyLinkNode*)malloc(sizeof(CycleDoublyLinkNode));
        newNode->data = data[i];
        newNode->next = dlk->next;
        newNode->pre = dlk;
        dlk->next->pre = newNode;
        dlk->next = newNode;
    }
}

/// @brief 尾插法建表
/// @param dlk
/// @param data
/// @param len
void CreateListR(CycleDoublyLinkNode*& dlk,ElemType data[],int len){
    dlk = (CycleDoublyLinkNode*)malloc(sizeof(CycleDoublyLinkNode));
    dlk->next = dlk->pre = dlk;
    CycleDoublyLinkNode* newNode;
    CycleDoublyLinkNode* ptrNode = dlk;
    for (int i = 0; i < len; i++){
        newNode = (CycleDoublyLinkNode*)malloc(sizeof(CycleDoublyLinkNode));
        newNode->data = data[i];
        ptrNode->next = newNode;
        newNode->pre = ptrNode;
        ptrNode = newNode;
    }
    ptrNode->next = dlk;
    dlk->pre = ptrNode;
}


int main()
{
    CycleDoublyLinkNode* list = NULL;
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

    // ================= 以下为边界测试（main 原来没覆盖的）=================
    // GetElem 的返回值先存进变量再 printf：
    // 写成 printf("%d %d", GetElem(list,4,&e), e) 的话，两个参数的求值顺序不确定
    int r;

    printf("\n=== 8. GetElem 边界 ===\n");
    list = NULL;
    InitList(list);
    InsElem(list,1,10);
    InsElem(list,2,20);
    InsElem(list,3,30);                     // 链表为 10 20 30
    PrintList(list);
    r = GetElem(list,3,&e);  printf("取第3个(最后一个,合法): 返回%d, e=%d\n",r,e);  // 期望：1, 30
    r = GetElem(list,4,&e);  printf("取第4个(恰好越界):      返回%d, e=%d\n",r,e);  // 期望：0
    r = GetElem(list,0,&e);  printf("取第0个(非法):          返回%d, e=%d\n",r,e);  // 期望：0
    r = GetElem(list,-1,&e); printf("取第-1个(非法):         返回%d, e=%d\n",r,e);  // 期望：0
    DestroyList(list);

    printf("\n=== 9. CreateListR 建空表(len=0) ===\n");
    list = NULL;
    CreateListR(list,array,0);
    printf("head->next == head ? %d\n",list->next==list);   // 期望：1
    printf("head->pre  == head ? %d\n",list->pre==list);    // 期望：1
    PrintList(list);                                        // 期望：空
    DestroyList(list);

    printf("\n=== 10. CreateListR 建表后头节点的 pre ===\n");
    list = NULL;
    CreateListR(list,array,len);            // 尾插 1 2 3 4 5
    PrintList(list);                        // 期望：1 2 3 4 5
    CycleDoublyLinkNode* tail = list;
    while(tail->next!=list)                 // 正向走一圈找到尾节点
        tail = tail->next;
    printf("head->next == head ? %d\n",list->next==list);   // 期望：0
    printf("head->pre  == 尾节点 ? %d\n",list->pre==tail);  // 期望：1
    // ⚠ 想看 head->pre 到底是不是野指针就解引用它，可能直接崩：
    // printf("head->pre 指回的 data = %d\n",list->pre->data);   // 期望：5
    DestroyList(list);

    printf("\n=== 11. DelElem 越界一位（放最后：它会毁掉链表）===\n");
    list = NULL;
    InitList(list);
    InsElem(list,1,10);
    InsElem(list,2,20);
    InsElem(list,3,30);                     // 长度 3
    PrintList(list);
    printf("删除第4个(长度只有3): 返回%d\n",DelElem(list,GetLength(list)+1));  // 期望：0
    // ⚠ 如果上面返回 1，说明它把"头节点"当成第4个节点删掉并 free 了。
    //   下面这行是删完之后唯一还安全的检查；一旦打出 0，链表已经废了：
    //   此时再调 PrintList / GetLength 会死循环（它们永远走不回"头节点"）。
    printf("头节点还在环里吗(head->next->pre == head): %d\n",list->next->pre==list);  // 期望：1
    // PrintList(list);     // ← 想看死循环现象就把这行注释打开（Ctrl+C 停）
    // DestroyList(list);   // ← 头节点已被 free，此时销毁会二次释放

    return 0;
}
