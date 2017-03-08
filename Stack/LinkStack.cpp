#include<stdio.h>
#include<stdlib.h>
/************************************************************************/
/*	链式栈的存储结构 
	2017年3月8日 13:56:10 
	Copyright Castile All Rights Reserved.                                                                  */
/************************************************************************/

typedef int ElemType;
typedef int Status;
/*Status是函数的类型，其值是函数结果状态代码，如OK等*/
#define MAXSIZE 10
#define TRUE  1
#define FALSE 0
#define  OK  1
#define  ERROR 0
#define  INFEASIBLE -1
#define OVERFLOW  -2

typedef struct Node //定义一个结构体变量
{
	ElemType data;
	struct Node* next;
}Node, *StackNode;

typedef struct Stack //定义一个栈
{
	StackNode top; //栈顶
	StackNode base; //栈底
	int count;//计数
}Stack , *LinkSatck; 


/*函数声明*/
Status InitStack(LinkSatck S); //初始化一个空栈
Status ClearStack(LinkSatck S); //将栈清空
Status StackEmpty(LinkSatck S); // 判断栈是否为空
Status GetTop(LinkSatck S, ElemType *e); //用e返回栈顶元素
Status Push(LinkSatck S, ElemType e); // 压栈
Status Pop(LinkSatck S, ElemType *e); //出栈，用e返回其值
Status StackLength(LinkSatck S); //返回栈的元素个数
void StackTravers(LinkSatck S); //遍历栈

//主函数
void main()
{
	Stack S;
	ElemType e;
	InitStack(&S);
	int a[10] = {1,2,3,4,5,6,7,8,9,10};
	
	for(int i=0;i<10;i++)
		Push(&S,a[i]);
	
	printf("栈中共有%d个元素。",StackLength(&S));
	GetTop(&S,&e);
	printf("栈顶元素是：%d\n",e);

	printf("遍历：\n");
	StackTravers(&S);
	printf("出栈：\n");
	while (!StackEmpty(&S))
	{
		Pop(&S,&e);
		printf("%d\t",e);
	}
	printf("\n出栈完成！\n");
	StackTravers(&S);
	printf("\n");	

}


/*函数实现*/
Status InitStack(LinkSatck S) //初始化一个空栈
{
	S->top = (StackNode)malloc(sizeof(Node));
	if(NULL == S->top)
	{
		printf("动态内存分配失败！\n");
		exit(OVERFLOW);
	}
	S->top = S->base;
	S->count = 0;
	return OK;
}

Status ClearStack(LinkSatck S) //将栈清空
{
	if(StackEmpty(S))
		return ERROR;
	StackNode p = S->top;
	StackNode q = NULL;
	while(p != S->base)
	{
		q = p->next;
		free(p);
		p = q;
	}
	S->top = S->base;
	return OK;
}

Status StackEmpty(LinkSatck S) //判断栈是否为空
{
	if(S->top == S->base)
		return TRUE;
	else
		return FALSE;
}
Status Push(LinkSatck S, ElemType e) // 压栈
{
	StackNode pNew = (StackNode)malloc(sizeof(Node));
	pNew->data = e;
	pNew->next = S->top; //把当前的栈顶元素赋给新节点的直接后继
	S->top = pNew; //新的结点赋给栈顶指针
	S->count++;
	return OK;
}

Status Pop(LinkSatck S, ElemType *e) //出栈，用e返回其值
{
	if(StackEmpty(S))
		return ERROR;
	*e = S->top->data;
	StackNode p = S->top;//将栈顶结点赋值给p
	S->top = S->top->next; //使得栈顶指针下移一位，指向后一结点
	free(p);  /*这条语句一定要放在后面！！！！不然会导致程序!崩溃非常重要*/
	S->count--;
	return OK;
}
Status StackLength(LinkSatck S)//返回栈的元素个数
{
	if(StackEmpty(S))
		return 0;
	return S->count;
}

Status GetTop(LinkSatck S, ElemType *e) //用e返回栈顶元素
{
	if(StackEmpty(S))
		return ERROR;
	*e = S->top->data;
	return OK;
}

void StackTravers(LinkSatck S)//遍历
{
	if(StackEmpty(S))
	{
		printf("栈为空！\n");
		return;
	}
	StackNode p = S->top;	
	while (p != S->base)
	{
		printf("%d\t",p->data);
		p = p->next;
	}

	printf("\n");

}


/************************************************************************/
/* 在VC++6.0中的运行结果为：

  栈中共有10个元素。栈顶元素是：10
  遍历：
  10      9       8       7       6       5       4       3       2       1
  出栈：
  10      9       8       7       6       5       4       3       2       1
  出栈完成！
  栈为空！
  
  Press any key to continue

	                                                                     */
/************************************************************************/
