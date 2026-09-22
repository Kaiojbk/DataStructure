#include<stdio.h>
#include<malloc.h>

// 字符链栈的基本操作（不带头结点，栈顶指针即头指针）

typedef char ElemType;
typedef struct node{
    ElemType data;
    struct node* next;
}LinkStackNode;

/// @brief 初始化链栈（构造空栈）
/// @param lst 栈顶指针
void InitStack(LinkStackNode*& lst){
    //  只需置空即可，入栈的时候再申请空间
    lst = NULL;
}

/// @brief 销毁链栈（释放全部结点）
/// @param lst 栈顶指针
void DestroyStack(LinkStackNode*& lst){
    if(lst==NULL)
        return;
    LinkStackNode* ptrNode = lst;
    LinkStackNode* freeNode;
    while(ptrNode!=NULL){
        freeNode = ptrNode;
        ptrNode = ptrNode->next;
        free(freeNode);
    }
    lst = NULL;
}

/// @brief 判空
/// @param lst 栈顶指针
/// @return 空栈返回 1，非空返回 0
int IsEmpty(LinkStackNode* lst){
    return lst == NULL;
}

/// @brief 获取栈中元素个数
/// @param lst 栈顶指针
/// @return 元素个数
int GetLength(LinkStackNode* lst){
    if(lst==NULL)
        return 0;
    int count = 0;
    LinkStackNode* ptrNode = lst;
    while(ptrNode!=NULL){
        count++;
        ptrNode = ptrNode->next;
    }
    return count;
}

/// @brief 取栈顶元素（不弹出）
/// @param lst 栈顶指针
/// @param value 通过指针带回栈顶元素
/// @return 成功返回 1，栈空返回 0
int GetTop(LinkStackNode* lst, ElemType* value){
    if(lst==NULL)
        return 0;
    *value = lst->data;
    return 1;
}

/// @brief 入栈
/// @param lst 栈顶指针
/// @param e 待入栈元素
/// @return 成功返回 1，失败返回 0
int Push(LinkStackNode*& lst, ElemType e){
    LinkStackNode* newNode = (LinkStackNode*)malloc(sizeof(LinkStackNode));
    newNode->data = e;
    newNode->next = lst;
    lst = newNode;
    return 1;
}

/// @brief 出栈
/// @param lst 栈顶指针
/// @param value 通过指针带回出栈元素
/// @return 成功返回 1，栈空返回 0
int Pop(LinkStackNode*& lst, ElemType* value){
    if(lst==NULL)
        return 0;
    LinkStackNode* tmpNode = lst;
    *value = lst->data;
    lst = lst->next;
    free(tmpNode);
    return 1; 
}

/// @brief 遍历栈（栈顶 -> 栈底）
/// @param lst 栈顶指针
void PrintStack(LinkStackNode* lst){
    LinkStackNode* ptrNode = lst;
    while(ptrNode!=NULL){
        printf("%c ",ptrNode->data);
        ptrNode = ptrNode->next;
    }
    putchar('\n');
}


int main()
{
    LinkStackNode* lst = NULL;
    ElemType e;
    char str[] = "HELLO";
    int len = sizeof(str)/sizeof(str[0])-1;       // 去掉末尾的 '\0'

    printf("=== 1. 初始化 ===\n");
    InitStack(lst);
    PrintStack(lst);                              // 期望：栈为空不可遍历
    printf("长度: %d\n",GetLength(lst));          // 期望：0
    printf("是否空栈: %d\n",IsEmpty(lst));        // 期望：1

    printf("\n=== 2. 入栈 ===\n");
    for(int i=0; i<len; i++){
        printf("入栈 %c : %d\n", str[i], Push(lst, str[i]));   // 期望：1
    }
    PrintStack(lst);                              // 期望：O L L E H
    printf("长度: %d\n",GetLength(lst));          // 期望：5

    printf("\n=== 3. 取栈顶 ===\n");
    if(GetTop(lst,&e))
        printf("栈顶: %c\n", e);                  // 期望：O
    printf("取完后长度: %d\n",GetLength(lst));    // 期望：5（取栈顶不影响栈）

    printf("\n=== 4. 出栈 ===\n");
    printf("出栈一次: %d\n", Pop(lst, &e));       // 期望：1
    printf("元素: %c\n", e);                      // 期望：O
    PrintStack(lst);                              // 期望：L L E H
    while(Pop(lst, &e))
        printf("出栈 %c\n", e);                   // 期望：L L E H
    printf("是否空栈: %d\n",IsEmpty(lst));        // 期望：1

    printf("\n=== 5. 空栈边界 ===\n");
    printf("空栈再出栈: %d\n", Pop(lst, &e));     // 期望：0
    printf("空栈取栈顶: %d\n", GetTop(lst, &e));  // 期望：0

    printf("\n=== 6. 销毁 ===\n");
    DestroyStack(lst);
    PrintStack(lst);                              // 期望：栈为空不可遍历
    printf("销毁后是否空栈: %d\n",IsEmpty(lst));  // 期望：1

    return 0;
}