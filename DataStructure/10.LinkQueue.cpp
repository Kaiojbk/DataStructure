#include<stdio.h>
#include<malloc.h>

//  实现链队的基本运算（带头结点的单向链队）
//  head->1->2->3


typedef char ElemType;
typedef struct node{
    ElemType data;
    struct node* next;
}QueueNode;

typedef struct {
    QueueNode* front;   // 永远指向头结点（不存数据，别把它的 data 当元素读）
    QueueNode* rear;    // 指向队尾结点；队空时和 front 一起指着头结点
}LinkQueue;

/// @brief 初始化队列（构造空队）
/// @param q 带回新建的队列
void InitQueue(LinkQueue *&q){
    q = (LinkQueue*)malloc(sizeof(LinkQueue));
    QueueNode* headNode = (QueueNode*)malloc(sizeof(QueueNode));
    headNode->next = NULL;
    q->front = q->rear = headNode;
}

/// @brief 销毁队列
/// @param q
void DestroyQueue(LinkQueue *&q){
    if(q==NULL)
        return;
    //  注意，本篇约定这个链队是带头节点的
    while(q->front!=q->rear){
        QueueNode* delNode = q->front;
        q->front = q->front->next;
        free(delNode);
    }
    free(q->rear);
    free(q);
    q=NULL;
}

/// @brief 判断队列是否为空
/// @param q
/// @return 空队返回 1，非空返回 0
int QueueEmpty(LinkQueue *q){
    return q->front==q->rear;
}

/// @brief 求队列中元素个数（不含头结点）
/// @param q
/// @return 元素个数
int QueueLength(LinkQueue *q){
    if(q==NULL||q->front==q->rear)
        return 0;
    int count = 0;
    QueueNode* ptrNode = q->front->next;
    while(ptrNode!=NULL){
        count++;
        ptrNode=ptrNode->next;
    }
    return count;
}

/// @brief 入队：把 e 放到队尾
/// @param q
/// @param e 要入队的元素
/// @return 成功返回 1，失败（malloc 失败）返回 0
int EnQueue(LinkQueue *q,ElemType e){
    if(q==NULL)
        return 0;
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if(newNode==NULL)
        return 0;
    newNode->data = e;
    newNode->next = NULL;
    q->rear->next = newNode;
    q->rear = newNode;
    return 1;
}

/// @brief 出队：删掉队头元素，并把它带出来
/// @param q
/// @param e 用来带回被删掉的队头元素
/// @return 成功返回 1，队空返回 0
int DeQueue(LinkQueue *q,ElemType *e){
    if(q==NULL||q->front==q->rear)
        return 0;
    *e = q->front->next->data;
    QueueNode* delNode = q->front->next;
    q->front->next = delNode->next;
    free(delNode);
    if(q->front->next==NULL)
        q->rear = q->front;
    return 1;
}

/// @brief 取队头元素：只看不删（取的是 front->next->data）
/// @param q
/// @param e 用来带回队头元素
/// @return 成功返回 1，队空返回 0
int GetHead(LinkQueue *q,ElemType *e){
    if(q==NULL||q->front==q->rear)
        return 0;
    *e = q->front->next->data;
    return 1;
}

/// @brief 从队头到队尾输出所有元素（头结点不打印）
/// @param q
void PrintQueue(LinkQueue *q){
    if(q==NULL||q->front==q->rear)
        return;
    QueueNode* ptrNode = q->front->next;
    do{
        printf("%c ",ptrNode->data);
        ptrNode=ptrNode->next;
    }while(ptrNode!=NULL);
    putchar('\n');
}

/// @brief 检查链队的内部不变量（白盒，测试用）
/// @param q
/// @return 结构自洽返回 1，否则返回 0
/// @note 专门抓"错位了一个结点"这类毛病。注意它抓不住"rear 悬空"
///       （指向已 free 的结点）—— 那块内存的内容通常还在，看着像没事，
///       悬空得靠用例 7 用行为去撞
int CheckInvariant(LinkQueue *q){
    if(q==NULL||q->front==NULL)
        return 0;                                   // 头结点必须一直存在
    QueueNode* ptrNode = q->front;
    int count = 0;
    while(ptrNode->next!=NULL){
        count++;
        ptrNode=ptrNode->next;
    }                                               // ptrNode 停在最后一个数据结点
    if(q->rear->next!=NULL)
        return 0;                                   // 队尾的 next 必须是 NULL
    if(count==0&&q->rear!=q->front)
        return 0;                                   // 空队：rear 要回指头结点
    if(count>0&&q->rear!=ptrNode)
        return 0;                                   // 非空：rear 要指向最后一个结点
    return count==QueueLength(q);                   // 数出来的个数要和 QueueLength 对得上
}

int main()
{
    setvbuf(stdout,NULL,_IONBF,0);  // 关掉输出缓冲：不然崩在这儿，崩溃前的打印全丢了，白跑
    LinkQueue *q = NULL;
    ElemType e;
    int r;      // 返回值先存进变量再打印：printf 多个参数的求值顺序是不确定的

    printf("=== 1. 初始化 ===\n");
    InitQueue(q);
    QueueNode *head = q->front;                 // 记住头结点，后面几组要检查它一直没动
    printf("q != NULL:          %d\n",q!=NULL);         // 期望：1
    printf("front 不是空指针:   %d\n",q->front!=NULL);  // 期望：1（头结点必须申请出来了）
    printf("front == rear:      %d\n",q->front==q->rear);// 期望：1（空队判据）
    printf("是否空队: %d\n",QueueEmpty(q));              // 期望：1
    printf("队长: %d\n",QueueLength(q));                 // 期望：0
    PrintQueue(q);                                       // 期望：什么都不输出（头结点不能被打印）
    printf("不变量: %d\n",CheckInvariant(q));            // 期望：1
    // ⚠ 现在 InitQueue 只申请了队列结构体、没建头结点，所以 front 那条是 0，
    //   紧接着第 2 组入队就会解引用空指针崩掉。先把它修了再往下跑。

    printf("\n=== 2. 入队 ===\n");
    printf("入队 a: %d\n",EnQueue(q,'a'));       // 期望：1
    printf("入队 b: %d\n",EnQueue(q,'b'));       // 期望：1
    printf("入队 c: %d\n",EnQueue(q,'c'));       // 期望：1
    PrintQueue(q);                                       // 期望：a b c
    printf("队长: %d（期望 3）\n",QueueLength(q));
    printf("是否空队: %d（期望 0）\n",QueueEmpty(q));
    printf("入队后 front 没动: %d（期望 1）\n",q->front==head);   // 入队只动 rear
    printf("队尾 next 是 NULL: %d（期望 1）\n",q->rear->next==NULL);// 不然遍历走不到头
    printf("不变量: %d（期望 1）\n",CheckInvariant(q));
    // ⚠ 最容易错的两头：头结点不能被当成元素打印出来，队尾那个 c 也不能漏掉

    printf("\n=== 3. 取队头不改队列 ===\n");
    r = GetHead(q,&e);  printf("取队头: 返回%d, e=%c\n",r,e);   // 期望：1, a
    GetHead(q,&e); GetHead(q,&e);               // 连着取两次，队列应该纹丝不动
    PrintQueue(q);                                       // 期望：还是 a b c
    printf("front 没动: %d（期望 1）\n",q->front==head);
    printf("队长: %d（期望 3）\n",QueueLength(q));

    printf("\n=== 4. 出队 ===\n");
    r = DeQueue(q,&e);  printf("出一次:   返回%d, e=%c\n",r,e); // 期望：1, a
    PrintQueue(q);                                       // 期望：b c
    printf("队长: %d（期望 2）\n",QueueLength(q));
    printf("front 还指着头结点: %d（期望 1）\n",q->front==head);
    r = DeQueue(q,&e);  printf("再出一次: 返回%d, e=%c\n",r,e); // 期望：1, b
    r = DeQueue(q,&e);  printf("再出一次: 返回%d, e=%c\n",r,e); // 期望：1, c
    printf("出完是否空队: %d（期望 1）\n",QueueEmpty(q));
    printf("出完后 rear 回指头结点: %d（期望 1）\n",q->rear==q->front);
    printf("不变量: %d（期望 1）\n",CheckInvariant(q));
    // 出队删的是 front->next（队头元素），不是 front 这个头结点本身；
    // 删掉最后一个元素时还要把 rear 拉回头结点，漏了这句 rear 就悬空了

    printf("\n=== 5. 空队时 DeQueue / GetHead 不能碰 e ===\n");
    e = '#';                                            // 先放个哨兵值进去
    r = DeQueue(q,&e);  printf("空队 DeQueue: 返回%d, e=%c\n",r,e);  // 期望：0, 还是 #
    e = '#';
    r = GetHead(q,&e);  printf("空队 GetHead: 返回%d, e=%c\n",r,e);  // 期望：0, 还是 #
    PrintQueue(q);                                       // 期望：什么都不输出
    // 返回 0 就说明压根没去读数据；要是返回 1，读到的就是头结点里的垃圾值
    printf("空队出完还能接着用: ");
    EnQueue(q,'x'); EnQueue(q,'y');
    PrintQueue(q);                                       // 期望：x y
    // ⚠ 空队出队要是把头结点自己删了，上面这两下就是在往已 free 的内存里写
    DeQueue(q,&e); DeQueue(q,&e);                        // 清干净，回到空队

    printf("\n=== 6. 验证先进先出 ===\n");
    ElemType in[] = {'1','2','3','4','5'};
    for (int i = 0; i < 5; i++)
        EnQueue(q,in[i]);
    PrintQueue(q);                                       // 期望：1 2 3 4 5
    printf("出队顺序: ");
    int guard = 0;                                       // 防止实现写错时在这儿转死
    while (!QueueEmpty(q)&&++guard<=20){
        DeQueue(q,&e);
        printf("%c ",e);
    }
    putchar('\n');                                       // 期望：1 2 3 4 5（和入队顺序一样）
    printf("出完队长: %d（期望 0）\n",QueueLength(q));
    // 和栈正好相反：栈是后进先出，队列是先进先出

    printf("\n=== 7. 删空后再接着用（撞 rear 悬空）===\n");
    printf("现在是否空队: %d（期望 1）\n",QueueEmpty(q));
    printf("删空后 rear 回指头结点: %d（期望 1）\n",q->rear==q->front);
    EnQueue(q,'c'); EnQueue(q,'d');                      // 删空之后再入队
    PrintQueue(q);                                       // 期望：c d
    printf("队长: %d（期望 2）\n",QueueLength(q));
    printf("不变量: %d（期望 1）\n",CheckInvariant(q));
    DeQueue(q,&e); DeQueue(q,&e);                        // 清干净，回到空队
    // 上一组是把队列删空的，rear 这时必须已经回到头结点；
    // 没回去的话 rear 就悬在已 free 的结点上，这里再入队就是往已释放的内存里写

    printf("\n=== 8. 单元素队列 ===\n");
    EnQueue(q,'a');
    printf("刚入完一个: 队长%d（期望 1）\n",QueueLength(q));
    r = GetHead(q,&e);  printf("取队头: 返回%d, e=%c\n",r,e);   // 期望：1, a
    PrintQueue(q);                                       // 期望：a
    r = DeQueue(q,&e);  printf("出一次: 返回%d, e=%c\n",r,e);  // 期望：1, a
    printf("是否空队: %d（期望 1）\n",QueueEmpty(q));
    printf("队长: %d（期望 0）\n",QueueLength(q));
    printf("不变量: %d（期望 1）\n",CheckInvariant(q));
    // 单元素是队尾 = 队头的临界情况，删它的时候 rear 最容易忘

    printf("\n=== 9. 反复 初始化 / 销毁 ===\n");
    for (int i = 1; i <= 3; i++){
        InitQueue(q);
        EnQueue(q,'a'); EnQueue(q,'b');
        r = DeQueue(q,&e);
        printf("第%d轮: 出了 %c\n",i,e);                 // 期望：每轮都是 a
        DestroyQueue(q);
        printf("第%d轮销毁后 q == NULL: %d\n",i,q==NULL); // 期望：1
    }
    // ⚠ 这组光看指针看不出内存问题：DestroyQueue 最后没 free 队列结构体本身的话，
    //   每轮泄漏 sizeof(LinkQueue)，打印出来一切正常。
    //   想坐实就在 main 开头加 _CrtDumpMemoryLeaks()（MSVC）或者用 valgrind 跑

    printf("\n=== 10. 随机对拍（拿数组当标准队列）===\n");
    InitQueue(q);
    ElemType model[256];        // 标准答案：一个最简单的数组队列
    int mHead = 0, mTail = 0;
    unsigned seed = 12345;
    int mismatch = 0;
    for (int i = 0; i < 200; i++){
        seed = seed*1103515245u+12345u;     // 自己造伪随机：不用 rand() 是为了每次结果都一样
        int op = (seed>>16)%2;
        if(op==0||mHead==mTail){            // 入队
            ElemType x = 'a'+(seed>>8)%26;
            EnQueue(q,x);
            model[mTail++] = x;
        }else{                              // 出队
            ElemType x = '?';
            r = DeQueue(q,&x);
            if(r!=1||x!=model[mHead])
                mismatch++;
            mHead++;
        }
        if(QueueLength(q)!=mTail-mHead)
            mismatch++;
    }
    printf("不匹配次数: %d（期望 0）\n",mismatch);
    printf("剩余队长: %d（期望 %d）\n",QueueLength(q),mTail-mHead);
    // 最后把剩下的全出出来，顺序也得和标准答案一样
    printf("剩余出队顺序: ");
    while (!QueueEmpty(q)&&mHead<mTail){
        DeQueue(q,&e);
        if(e!=model[mHead])
            mismatch++;
        mHead++;
        printf("%c",e);
    }
    putchar('\n');
    printf("对拍结束不匹配次数: %d（期望 0）\n",mismatch);
    // 边界不用自己想，机器替你想：哪一步开始对不上，那里就是 bug

    DestroyQueue(q);

    return 0;
}
