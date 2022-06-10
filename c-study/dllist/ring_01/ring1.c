#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
//用链表实现队列
//节点结构
typedef struct Node {
    struct Node *next;
    int data;
} node;
//队列的定义
typedef struct Queue {
    node *front;
    node *rear;
} queue;
//定义一个全局的队列
queue que;
//队列的初始化操作
void QueInit(queue * que)
{
    //申请一个新的节点
    node *temp = (node *) malloc(sizeof(node));
    assert(temp != NULL);
    temp->next = NULL;
    que->front = que->rear = temp;
}

//队空判断
int QueEmpty()
{
    return __sync_bool_compare_and_swap(&(que.rear), que.front, que.front);
}

//入队操作
void QuePush(int *d)
{
    //申请新节点
    node *temp = (node *) malloc(sizeof(node));
    assert(temp != NULL);
    temp->data = (*d);
    temp->next = NULL;
    //将新申请的节点利用原子操作插入到队列当中
    node *p;
    do {
        p = que.rear;
    }
    while (!__sync_bool_compare_and_swap(&(p->next), NULL, temp));
    //重置尾指针
    __sync_bool_compare_and_swap(&(que.rear), p, temp);
}

//出队操作
int QuePop(int *d)
{
    //temp为要输出的元素
    node *temp;
    //因为temp可能为NULL，因此我们用P记录temp->next的值，后续会用到
    node *p;

    do {
        if (QueEmpty())
            return 0;
        temp = que.front->next;
        if (temp != NULL)
            p = temp->next;
        else
            p = NULL;
    }
    while (!__sync_bool_compare_and_swap(&(que.front->next), temp, p));
    //当尾指针也指向当前要出队的结点时，更新尾指针
    __sync_bool_compare_and_swap(&(que.rear), temp, que.front);
    if (temp != NULL) {
        *d = temp->data;
        free(temp);
        return 1;
    }
    return 0;
}

//两个线程函数：一个入队，一个出队
void *thread_push(void *arg)
{
    int c = 0;
    while (c < 10) {
        int data = rand() % 100;
        QuePush(&data);
        printf("队列插入元素：%d\n", data);
        sleep(1);
        c++;
    }
    return NULL;
}

void *thread_pop(void *arg)
{
    int data;
    int c = 0;
    while (c < 12) {
        sleep(2);
        if (!QuePop(&data))
            printf("队列为空\n");
        else
            printf("队列输出元素：%d\n", data);
        c++;
    }
    return NULL;
}

int main()
{
    //初始化队列
    QueInit(&que);
    //创建两个线程
    pthread_t id[2];
    pthread_create(&id[0], NULL, thread_push, NULL);
    pthread_create(&id[1], NULL, thread_pop, NULL);
    //等待线程结束
    pthread_join(id[0], NULL);
    pthread_join(id[1], NULL);
    //在这之后还因该删除队列回收内存
    //删除队列不涉及多线程操作，不再赘述
    return 0;
}
