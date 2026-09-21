#include<stdio.h>
#include<malloc.h>
#define MaxSize 100

//  实现顺序栈的基本运算
//  约定：
//   1. top 是"栈顶元素的下标"：空栈时 top == -1，栈满时 top == MaxSize-1
//   2. 行参继续沿用上一篇的 *&（指针的引用）：函数要改的就是"调用方那个指针本身"，
//      调用时直接写 st，不用像前面几篇那样写 &st
//   3. data 数组是定长的，但栈本身还是 malloc 出来的：
//      销毁时只要 free(s) 一下就完事，不用像链表那样逐个节点释放

typedef char ElemType;
typedef struct{
    ElemType data[MaxSize];
    int top;
}SequenceStack;

/// @brief 初始化栈
/// @param s
void InitStack(SequenceStack *&s){
    s = (SequenceStack*)malloc(sizeof(SequenceStack));
    s->top = -1;
}

/// @brief 销毁栈
/// @param s
void DestroyStack(SequenceStack *&s){
    if(s==NULL)
        return;
    free(s);
    s = NULL;
}

/// @brief 判断栈是否为空
/// @param s
/// @return 空栈返回 1，非空返回 0
int StackEmpty(SequenceStack *s){
    return s->top==-1;
}

/// @brief 进栈：把 e 放到栈顶
/// @param s
/// @param e 要进栈的元素
/// @return 成功返回 1，栈满返回 0
int Push(SequenceStack *s,ElemType e){
    //  top范围0-99
    //  有1个元素时，top=0;有2个元素时，top=1;
    if(s->top == MaxSize-1)
        return 0;
    s->data[++s->top]=e;       // top从-1开始，先移动指针再赋值
    return 1;
}

/// @brief 出栈：删掉栈顶元素，并把它带出来
/// @param s
/// @param e 用来带回被删掉的栈顶元素
/// @return 成功返回 1，栈空返回 0
int Pop(SequenceStack *s,ElemType *e){
    if(s->top==-1)
        return 0;
    *e = s->data[s->top--];
    return 1;
}

/// @brief 取栈顶元素：只看不删
/// @param s
/// @param e 用来带回栈顶元素
/// @return 成功返回 1，栈空返回 0
int GetTop(SequenceStack *s,ElemType *e){
    if(s->top==-1)
        return 0;
    *e = s->data[s->top];
    return 1;
}

/// @brief 从栈底到栈顶输出所有元素（顺便看看数据在数组里是怎么排的）
/// @param s
void PrintStack(SequenceStack *s){
    if(s->top==-1)
        return;
    for (int i = 0; i <= s->top; i++)
        printf("%c ",s->data[i]);
    putchar('\n');
}

int main()
{
    SequenceStack *st = NULL;
    ElemType e;
    int r;      // 返回值先存进变量再打印：printf 多个参数的求值顺序是不确定的

    printf("=== 1. 初始化 ===\n");
    InitStack(st);
    printf("栈顶下标 top: %d\n",st->top);       // 期望：-1
    printf("是否空栈: %d\n",StackEmpty(st));    // 期望：1
    PrintStack(st);                             // 期望：提示栈空，别去访问 data[-1]

    printf("\n=== 2. 进栈 ===\n");
    printf("压入 a: %d\n",Push(st,'a'));        // 期望：1
    printf("压入 b: %d\n",Push(st,'b'));        // 期望：1
    printf("压入 c: %d\n",Push(st,'c'));        // 期望：1
    PrintStack(st);                             // 期望：a b c（栈底到栈顶）
    printf("top: %d\n",st->top);                // 期望：2
    printf("是否空栈: %d\n",StackEmpty(st));    // 期望：0
    r = GetTop(st,&e);  printf("取栈顶: 返回%d, e=%c\n",r,e);   // 期望：1, c

    printf("\n=== 3. 取栈顶不改栈 ===\n");
    GetTop(st,&e); GetTop(st,&e);               // 连着取两次，栈应该纹丝不动
    PrintStack(st);                             // 期望：还是 a b c
    printf("top: %d\n",st->top);                // 期望：2

    printf("\n=== 4. 出栈 ===\n");
    r = Pop(st,&e);     printf("弹一次:   返回%d, e=%c\n",r,e);  // 期望：1, c
    PrintStack(st);                             // 期望：a b
    r = Pop(st,&e);     printf("再弹一次: 返回%d, e=%c\n",r,e);  // 期望：1, b
    r = Pop(st,&e);     printf("再弹一次: 返回%d, e=%c\n",r,e);  // 期望：1, a
    printf("出完是否空栈: %d\n",StackEmpty(st));// 期望：1
    r = Pop(st,&e);     printf("空栈再弹: 返回%d\n",r);          // 期望：0
    printf("top: %d\n",st->top);                // 期望：-1

    printf("\n=== 5. 验证后进先出 ===\n");
    ElemType in[] = {'1','2','3','4','5'};
    for (int i = 0; i < 5; i++)
        Push(st,in[i]);
    PrintStack(st);                             // 期望：1 2 3 4 5
    printf("出栈顺序: ");
    while (!StackEmpty(st)){
        Pop(st,&e);
        printf("%c ",e);
    }
    putchar('\n');                              // 期望：5 4 3 2 1（和进栈顺序正好相反）

    printf("\n=== 6. 空栈时 Pop / GetTop 不能碰 e ===\n");
    e = '#';                                        // 先放个哨兵值进去
    r = Pop(st,&e);     printf("空栈 Pop:    返回%d, e=%c\n",r,e);  // 期望：0, 还是 #
    e = '#';
    r = GetTop(st,&e);  printf("空栈 GetTop: 返回%d, e=%c\n",r,e);  // 期望：0, 还是 #
    // 返回 0 就说明压根没去读 data[top]：空栈时 top == -1，读它已经越界了

    printf("\n=== 7. 单元素栈 ===\n");
    printf("压入 a: %d\n",Push(st,'a'));        // 期望：1
    r = GetTop(st,&e);  printf("取栈顶: 返回%d, e=%c\n",r,e);       // 期望：1, a
    PrintStack(st);                             // 期望：a
    r = Pop(st,&e);     printf("弹一次: 返回%d, e=%c\n",r,e);       // 期望：1, a
    printf("是否空栈: %d\n",StackEmpty(st));    // 期望：1

    printf("\n=== 8. 应用：字符串逆序（只用 Push / Pop / StackEmpty）===\n");
    char str[] = "Hello Stack";
    char rev[sizeof(str)];                      // 和 str 一样大，装得下最后的 '\0'
    for (int i = 0; str[i] != '\0'; i++)
        Push(st,str[i]);
    int k = 0;
    while (!StackEmpty(st))
        Pop(st,&rev[k++]);
    rev[k] = '\0';                              // 长度一样，k 最后正好是原串长度
    printf("原串: %s\n",str);
    printf("逆序: %s\n",rev);                   // 期望：kcatS olleH
    DestroyStack(st);

    printf("\n=== 9. 反复 初始化 / 销毁 ===\n");
    for (int i = 1; i <= 3; i++){
        InitStack(st);
        Push(st,'a'); Push(st,'b');
        r = Pop(st,&e);
        printf("第%d轮: 弹出了 %c\n",i,e);      // 期望：每轮都是 b
        DestroyStack(st);
        printf("第%d轮销毁后 st == NULL: %d\n",i,st==NULL);   // 期望：1
    }
    // 销毁时没置空的话这里就露馅了：st 还指着已经 free 掉的那块内存，
    // 下一轮 InitStack 直接把它盖掉 —— 旧的那块就泄漏了

    printf("\n=== 10. 反复 InitStack 建同一个 st（不该泄漏）===\n");
    InitStack(st);
    e = 'a';
    // ⚠ 下面这行只是在演示"忘了销毁"的现象，跑完记得注释掉：
    // InitStack(st);   // ← 打开它：旧的那块内存就没人管了（泄漏），而且 st 被换成了新地址
    Push(st,e);
    PrintStack(st);                             // 期望：a
    DestroyStack(st);

    printf("\n=== 11. 栈满（⚠ 放最后：Push 要是漏判栈满，这里会越界写）===\n");
    InitStack(st);
    int pushed = 0;
    for (int i = 0; i < MaxSize; i++){
        if (!Push(st,'x')){
            printf("第 %d 个就压不进去了\n",i+1);
            break;
        }
        pushed++;
    }
    printf("成功压入 %d 个（期望 %d）\n",pushed,MaxSize);
    printf("压满后 top: %d（期望 %d）\n",st->top,MaxSize-1);
    printf("栈满再压: %d\n",Push(st,'y'));      // 期望：0
    r = GetTop(st,&e);  printf("满栈取栈顶: 返回%d, e=%c\n",r,e);   // 期望：1, x
    r = Pop(st,&e);     printf("弹一个: 返回%d, e=%c\n",r,e);       // 期望：1, x
    printf("弹一个后再压入 y: %d\n",Push(st,'y'));                  // 期望：1
    r = GetTop(st,&e);  printf("新的栈顶: %c\n",e);                 // 期望：y
    printf("top 还是: %d（期望 %d）\n",st->top,MaxSize-1);
    DestroyStack(st);
    // ⚠ 漏判栈满的话，data[MaxSize] 就写到数组外面去了：这个结构体里 data 后面
    //   紧挨着的正是 top，所以越界会直接把 top 写坏，后面所有输出全乱套

    return 0;
}
