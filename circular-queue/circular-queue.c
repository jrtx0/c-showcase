#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int front;
    int rear;
} CircularQueue;

void initQueue(CircularQueue *queue)
{
    queue->front = 0;
    queue->rear = 0;
}

int isEmpty(CircularQueue *queue)
{
    return queue->front == queue->rear;
}

int isFull(CircularQueue *queue)
{
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

int enqueue(CircularQueue *queue, int value)
{
    if (isFull(queue))
    {
        return 0;
    }

    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % MAX_SIZE;
}

int dequeue(CircularQueue *queue, int *value)
{
    if (isEmpty(queue))
    {
         return 0; 
    }

    *value = queue->data[queue->front];
    queue->front = (queue->front + 1) % MAX_SIZE;
    return 1;
}

int main(void)
{
    CircularQueue queue;
    initQueue(&queue); 

    printf("初始化队列完成。\n");
    printf("队列是否为空：%s\n", isEmpty(&queue) ? "是" : "否");

    printf("开始出队操作...\n");
    int value;
    for (int i = 0; i < 3; i++)
    {
        if (dequeue(&queue, &value))
	{
	    printf("出队成功：%d\n", value);
	}
	else
	{
	    printf("出队失败：队列为空。\n");
	}
    }

    printf("队列是否为空：%s\n", isEmpty(&queue) ? "是" : "否");
    printf("队列是否为满：%s\n", isFull(&queue) ? "是" : "否");

    printf("再次尝试入队...\n");
    for (int i = 6; i <= 8; i++)
    {
        if (enqueue(&queue, i)) 
	{
            printf("入队成功：%d\n", i);	     
	}
	else
	{
	    printf("入队失败：队列已满。\n");
	}
    }

    printf("继续出队，直到队列为空...\n");
    while (dequeue(&queue, &value))
    {
         printf("出队成功：%d\n", value);
    }

    printf("队列是否为空：%s\n", isEmpty(&queue) ? "是" : "否");

    return 0;
}
