#include<stdio.h>
#include<malloc.h>
#define MaxSize 100

//  实现循环队列的基本运算
//  约定：
//   1. front 是"队头元素的下标"，rear 是"队尾元素的下一个位置"：
//      空队时 front == rear，队尾入队、队头出队
//   2. 用"牺牲一个存储单元"的办法区分队空和队满：
//      队空：front == rear
//      队满：(rear+1) % MaxSize == front
//      所以队列最多只能放 MaxSize-1 个元素

typedef char ElemType;
typedef struct{
    ElemType data[MaxSize];
    int front;      // 队头元素的下标
    int rear;       // 队尾元素的下一个位置
}CycleQueue;

/// @brief 初始化队列
/// @param cq
void InitQueue(CycleQueue *&cq){
    cq = (CycleQueue*)malloc(sizeof(CycleQueue));
    cq->front = cq->rear = 0;
}

/// @brief 销毁队列
/// @param cq
void DestroyQueue(CycleQueue *&cq){
    free(cq);
    cq = NULL;
}

/// @brief 判断队列是否为空
/// @param cq
/// @return 空队返回 1，非空返回 0
int QueueEmpty(CycleQueue *cq){
    return cq->front == cq->rear;
}

/// @brief 求队列中元素个数
/// @param cq
/// @return 元素个数
int QueueLength(CycleQueue *cq){
    if(cq==NULL)
        return 0;
    return (cq->rear-cq->front+MaxSize)%MaxSize;
}

/// @brief 入队：把 e 放到队尾
/// @param cq
/// @param e 要入队的元素
/// @return 成功返回 1，队满返回 0
int EnQueue(CycleQueue *cq,ElemType e){
    if((cq->rear+1)%MaxSize==cq->front)
        return 0;
    cq->data[cq->rear] = e;
    cq->rear = (cq->rear+1)%MaxSize;
    return 1;
}

/// @brief 出队：删掉队头元素，并把它带出来
/// @param cq
/// @param e 用来带回被删掉的队头元素
/// @return 成功返回 1，队空返回 0
int DeQueue(CycleQueue *cq,ElemType *e){
    if(cq==NULL||cq->front==cq->rear)
        return 0;
    *e = cq->data[cq->front];
    cq->front = (cq->front+1)%MaxSize;
    return 1;
}

/// @brief 取队头元素：只看不删
/// @param cq
/// @param e 用来带回队头元素
/// @return 成功返回 1，队空返回 0
int GetHead(CycleQueue *cq,ElemType *e){
    if(cq==NULL||cq->front==cq->rear)
        return 0;
    *e = cq->data[cq->front];
    return 1;
}

/// @brief 从队头到队尾输出所有元素（顺便看看数据在数组里是怎么绕的）
/// @param cq
void PrintQueue(CycleQueue *cq){
    if(cq==NULL)
        return;
    // != 判终点，绕回的事交给取模
    for(int i = cq->front;i!=cq->rear;i=(i+1)%MaxSize)
        printf("%c ",cq->data[i]);
    putchar('\n');
}

int main()
{
    CycleQueue *q = NULL;
    ElemType e;
    int r;      // 返回值先存进变量再打印：printf 多个参数的求值顺序是不确定的

    printf("=== 1. 初始化 ===\n");
    InitQueue(q);
    printf("队头下标 front: %d\n",q->front);    // 期望：0
    printf("队尾下标 rear:  %d\n",q->rear);     // 期望：0
    printf("是否空队: %d\n",QueueEmpty(q));     // 期望：1
    printf("队长: %d\n",QueueLength(q));        // 期望：0
    PrintQueue(q);                              // 期望：能正常结束，别输出乱码

    printf("\n=== 2. 入队 ===\n");
    printf("入队 a: %d\n",EnQueue(q,'a'));      // 期望：1
    printf("入队 b: %d\n",EnQueue(q,'b'));      // 期望：1
    printf("入队 c: %d\n",EnQueue(q,'c'));      // 期望：1
    PrintQueue(q);                              // 期望：a b c（队头到队尾）
    printf("front: %d, rear: %d\n",q->front,q->rear);  // 期望：0, 3
    printf("队长: %d\n",QueueLength(q));        // 期望：3
    printf("是否空队: %d\n",QueueEmpty(q));     // 期望：0
    printf("入队后 front 没动: %d\n",q->front); // 期望：0（入队只动 rear）

    printf("\n=== 3. 取队头不改队列 ===\n");
    r = GetHead(q,&e);  printf("取队头: 返回%d, e=%c\n",r,e);   // 期望：1, a
    GetHead(q,&e); GetHead(q,&e);               // 连着取两次，队列应该纹丝不动
    PrintQueue(q);                              // 期望：还是 a b c
    printf("front: %d, rear: %d\n",q->front,q->rear);  // 期望：0, 3

    printf("\n=== 4. 出队 ===\n");
    r = DeQueue(q,&e);  printf("出一次:   返回%d, e=%c\n",r,e); // 期望：1, a
    PrintQueue(q);                              // 期望：b c
    printf("front: %d, rear: %d\n",q->front,q->rear);  // 期望：1, 3（出队只动 front）
    r = DeQueue(q,&e);  printf("再出一次: 返回%d, e=%c\n",r,e); // 期望：1, b
    r = DeQueue(q,&e);  printf("再出一次: 返回%d, e=%c\n",r,e); // 期望：1, c
    printf("出完是否空队: %d\n",QueueEmpty(q)); // 期望：1
    r = DeQueue(q,&e);  printf("空队再出: 返回%d\n",r);         // 期望：0

    printf("\n=== 5. 空队时 DeQueue / GetHead 不能碰 e ===\n");
    e = '#';                                        // 先放个哨兵值进去
    r = DeQueue(q,&e);  printf("空队 DeQueue: 返回%d, e=%c\n",r,e);  // 期望：0, 还是 #
    e = '#';
    r = GetHead(q,&e);  printf("空队 GetHead: 返回%d, e=%c\n",r,e);  // 期望：0, 还是 #
    // 返回 0 就说明压根没去读 data[front]；队空时 front == rear，
    // 读到的其实是那个"牺牲掉"的单元，或者是早就该扔掉的老数据（假元素）

    printf("\n=== 6. 验证先进先出 ===\n");
    ElemType in[] = {'1','2','3','4','5'};
    for (int i = 0; i < 5; i++)
        EnQueue(q,in[i]);
    PrintQueue(q);                              // 期望：1 2 3 4 5
    printf("出队顺序: ");
    while (!QueueEmpty(q)){
        DeQueue(q,&e);
        printf("%c ",e);
    }
    putchar('\n');                              // 期望：1 2 3 4 5（和入队顺序一样）
    // 和栈正好相反：栈是后进先出，队列是先进先出

    printf("\n=== 7. 反复 初始化 / 销毁 ===\n");
    for (int i = 1; i <= 3; i++){
        InitQueue(q);
        EnQueue(q,'a'); EnQueue(q,'b');
        r = DeQueue(q,&e);
        printf("第%d轮: 出了 %c\n",i,e);        // 期望：每轮都是 a
        DestroyQueue(q);
        printf("第%d轮销毁后 q == NULL: %d\n",i,q==NULL);   // 期望：1
    }
    // 销毁时没置空的话这里就露馅了：q 还指着已经 free 掉的那块内存，
    // 下一轮 InitQueue 直接把它盖掉 —— 旧的那块就泄漏了

    printf("\n=== 8. 单元素队列 ===\n");
    InitQueue(q);
    EnQueue(q,'a');
    r = GetHead(q,&e);  printf("取队头: 返回%d, e=%c\n",r,e);   // 期望：1, a
    PrintQueue(q);                              // 期望：a
    printf("front: %d, rear: %d\n",q->front,q->rear);  // 期望：0, 1
    r = DeQueue(q,&e);  printf("出一次: 返回%d, e=%c\n",r,e);  // 期望：1, a
    printf("是否空队: %d\n",QueueEmpty(q));     // 期望：1
    DestroyQueue(q);

    printf("\n=== 9. 循环绕回（队列的重点，别让它变成假溢出）===\n");
    InitQueue(q);
    for (int i = 1; i <= MaxSize-1; i++)        // 最多装 MaxSize-1 个
        EnQueue(q,'a' + (i-1) % 26);            // 依次入队 a b c ... 绕一圈字母
    printf("入满后 front: %d, rear: %d\n",q->front,q->rear);   // 期望：0, 99
    printf("队长: %d（期望 %d）\n",QueueLength(q),MaxSize-1);  // 期望：99
    for (int i = 0; i < 2; i++)                 // 出掉两个，给后面腾位置
        DeQueue(q,&e);
    printf("出两个后 front: %d\n",q->front);    // 期望：2
    printf("再入队两个: %d %d\n",EnQueue(q,'x'),EnQueue(q,'y'));  // 期望：1 1
    printf("绕回后 rear: %d\n",q->rear);        // 期望：1
    // rear 从 99 加 1 就该是 100：越界了！必须 %MaxSize 让它绕回 0。
    // 忘了取模的话，rear 会一直往数组外面跑，PrintQueue 也走不到头
    r = GetHead(q,&e);  printf("队头还是: %c（期望 c）\n",e);   // 期望：c
    printf("队长: %d（期望 %d）\n",QueueLength(q),MaxSize-1);  // 期望：99
    DestroyQueue(q);
    // ⚠ 这里只绕回了一次；想看得更清楚可以再出两个、再入两个，
    //   看 rear 从 1 接着走到 2、3……

    printf("\n=== 10. 队满（⚠ 放最后：EnQueue 要是漏判队满，这里会越界写）===\n");
    InitQueue(q);
    int enqueued = 0;
    for (int i = 0; i < MaxSize; i++){          // 故意多塞一个
        if (!EnQueue(q,'x')){
            printf("第 %d 个就进不去了\n",i+1);  // 期望：第 100 个
            break;
        }
        enqueued++;
    }
    printf("成功入队 %d 个（期望 %d）\n",enqueued,MaxSize-1);   // 期望：99
    printf("队满后 front: %d, rear: %d\n",q->front,q->rear);    // 期望：0, 99
    printf("队满再入: %d\n",EnQueue(q,'y'));                    // 期望：0
    r = GetHead(q,&e);  printf("满队取队头: 返回%d, e=%c\n",r,e);   // 期望：1, x
    r = DeQueue(q,&e);  printf("出一个: 返回%d, e=%c\n",r,e);       // 期望：1, x
    printf("出一个后再入队 y: %d\n",EnQueue(q,'y'));                // 期望：1
    r = GetHead(q,&e);  printf("新的队头: %c\n",e);                 // 期望：x
    printf("rear 绕到: %d\n",q->rear);          // 期望：0（99 再往后一位就绕回开头了）
    printf("队长: %d（期望 %d）\n",QueueLength(q),MaxSize-1);       // 期望：99
    DestroyQueue(q);
    // ⚠ 漏判队满的话，data[MaxSize] 就写到数组外面去了；而且队满时 front == rear
    //   和队空长得一模一样，判空判满判错，出的就是"假元素"
    // ⚠ MaxSize 个元素只能装下 MaxSize-1 个，这是"牺牲一个单元"的代价：
    //   想装满就得再开个变量记元素个数，或者加个 tag 标志

    return 0;
}
