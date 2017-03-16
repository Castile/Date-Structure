#include<stdio.h>
#include<stdlib.h>
/************************************************************************/
/* 循环队列
   2017年3月16日 20:04:06      Castile                                                                */
/************************************************************************/

/*
	约定：初始化建立空队列时，令front=rear=0，每当插入新的队列元素时，“尾指针增加1”；每当删除队列头元素时，“头指针增1”
		  因此，在非空队列中，头指针始终指向队列头元素，而尾指针始终指向队尾元素的下一个位置


  队列空的条件：Q->front == Q->rear
  队列满的条件：(Q->rear+1)%MAXQSIZE == Q->front
  下一个元素：(Q->front+1)%MAXQSIZE
*/

#define TRUE		1
#define FALSE		0
#define OK		  1
#define ERROR		0
#define INFEASIBLE	-1
#define OVERFLOW	-2
#define MAXQSIZE	100
typedef int Status;
typedef int QElemType;

typedef struct
{
	QElemType *base;
	int front;
	int rear;
}SqQueue;

//函数接口
Status InitQueue(SqQueue *Q); //构造一个空队列
Status QueueLength(SqQueue Q);//返回元素的个数，即队列的长度
Status EnQueue(SqQueue *Q, QElemType e);//入队
Status DeQueue(SqQueue *Q, QElemType *e);//出队
Status GetHead(SqQueue Q);//获取首元素
Status TraverseQueue(SqQueue Q); //遍历队列
Status DestroyQueue(SqQueue *Q); //销毁队列

void main()
{
	SqQueue sqQue;		//队列变量
	int count;
	QElemType e;
	InitQueue( &sqQue );
	count = QueueLength( sqQue );
	printf("队列中元素的个数：%d \n", count );
	for( int i = 0; i < 30; i++ )
		EnQueue( &sqQue, i );
	TraverseQueue(sqQue);
	count = QueueLength( sqQue );
	printf("队列中元素的个数：%d \n", count );
	DeQueue( &sqQue, &e );
	printf("出队列的元素是：%d \n", e );
	count = QueueLength( sqQue );
	printf("队列中元素的个数：%d \n", count );


}

Status InitQueue(SqQueue *Q)
{
	Q->base = (QElemType *)malloc(MAXQSIZE*sizeof(QElemType));
	if(!Q->base)
		exit(OVERFLOW);
	Q->front = Q->rear = 0;
	return OK;
}
Status GetHead(SqQueue Q)
{
	QElemType e;
	if(Q.front == Q.rear)
		return ERROR;
	return e = Q.base[Q.front];
}

Status QueueLength(SqQueue Q)
{
	return (Q.rear-Q.front+MAXQSIZE)%MAXQSIZE;
}

Status EnQueue(SqQueue *Q, QElemType e)
{
	if((Q->rear+1)%MAXQSIZE == Q->front) //队列满
		return ERROR;
	Q->base[Q->rear] = e;
	Q->rear = (Q->rear+1)%MAXQSIZE;
	return OK;
}

Status DeQueue(SqQueue *Q ,QElemType *e)
{
	if(Q->front == Q->rear) //队列空
		return ERROR;
	*e = Q->base[Q->front];
	Q->front = (Q->front+1)%MAXQSIZE;
	return OK;
}

Status DestroyQueue(SqQueue *Q)
{
	while ( Q->front!= Q->rear)
	{
		free(&Q->base[Q->front]);
		Q->front=(Q->front+1)%MAXQSIZE;
	}
	return OK;
}

Status TraverseQueue(SqQueue Q)
{
	int i = Q.front;
	while(i != Q.rear)
	{
		printf("%d	",Q.base[i]);
		i = i+1;
	}
	return OK;

}


/************************************************************************/
/* 在VC++6.0的运行的结果为：


  队列中元素的个数：0
  0       1       2       3       4       5       6       7       8       9       10      11      12      13      14
  15      16      17      18      19      20      21      22      23      24      25      26      27      28      29
  队列中元素的个数：30
  出队列的元素是：0
  队列中元素的个数：29
Press any key to continue                                                                     */
/************************************************************************/
