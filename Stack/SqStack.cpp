#include <stdio.h>
#include <stdlib.h>
/************************************************************************/
/* 顺序栈的存储结构 -->顺序存储（数组实现）
2017年3月7日 21:44:59  Castile All Rights Reserve
栈(Stack):是限定仅在表尾进行插入和删除操作的线性表
特点：先进后出(Last in first out)

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

typedef struct Stack {
	ElemType data[MAXSIZE];
	int top; //栈顶
}SqStack;
/*当栈存在一个元素时，top等于0，因此通常把空栈的判定条件定为top = -1*/


/*函数声明*/
Status InitStack(SqStack *S); //初始化操作，建立一个空栈S
Status ClearStack(SqStack *S); //将栈清空
Status StackEmpty(SqStack S); // 判断栈是否为空
Status GetTop(SqStack S, ElemType *e); //用e返回栈顶元素
Status Push(SqStack *S, ElemType e); // 压栈
Status Pop(SqStack *S, ElemType *e); //出栈，用e返回其值
Status StackLength(SqStack S); //返回栈的元素个数
void StackTravers(SqStack S); //遍历栈

void main()
{
	SqStack S;
	ElemType e;
	InitStack(&S);
	Push(&S, 1);
	Push(&S, 2);
	Push(&S, 3);
	Push(&S, 4);
	Push(&S, 5);
	int length = StackLength(S);
	printf("该栈中共有%d个元素！\n",length);
	int val ;
	GetTop(S,&val);
	printf("栈顶元素是：%d\n",val);
//	StackTravers(S);
 	while(!StackEmpty(S))
 	{
 		Pop(&S,&e);
 		printf("%d\t",e);
 
	}
	printf("\n");
	length = StackLength(S);
	printf("全部出栈，该栈中共有%d个元素！\n",length);

}



/*函数实现*/
Status InitStack(SqStack *S)//初始化操作，建立一个空栈S
{
	S->top = -1;
	//S->data[MAXSIZE] = { 0 };
	return OK;

}

Status StackEmpty(SqStack S)// 判断栈是否为空
{
	if (S.top == -1)
		return TRUE;
	else
		return FALSE;
}

Status ClearStack(SqStack *S) //将栈清空
{
	S->top = -1;
	return OK;
}

Status Push(SqStack *S, ElemType e)// 压栈
{
	if (S->top == MAXSIZE-1) //栈满
		return ERROR;
	S->top++;
	S->data[S->top] = e;
	return OK;
}
Status Pop(SqStack *S, ElemType *e) //出栈，用e返回其值
{
	if (S->top == -1)
		return ERROR;
	*e = S->data[S->top]; //将要删除的栈顶元素赋值给e
	S->top--; //栈顶指针减一
	return OK;
}
Status GetTop(SqStack S, ElemType *e)//用e返回栈顶元素
{
	if (StackEmpty(S))
		return ERROR;
	*e = S.data[S.top];
	return OK;
}
Status StackLength(SqStack S)
{
 	if (StackEmpty(S))
 		return 0;
	return S.top + 1; 
}
void StackTravers(SqStack S) //遍历栈
{
	if (StackEmpty(S))
 		printf("栈为空！\n");
	for (int i = S.top ; i > -1; i--)
 	{
 		printf("%d\t", S.data[i]);
 	}
 	printf("\n");

	/*
	另一种方式：

		while(!StackEmpty(S))
		{
			Pop(&S,&e);
			printf("%d\t",e);
		
		  }
		printf("\n");
	
	*/
 }
 
 
/************************************************************************/
/* 在VC++6.0中的运行结果是：
该栈中共有5个元素！
栈顶元素是：5
5       4       3       2       1
全部出栈，该栈中共有0个元素！
Press any key to continue                                                                     */
/************************************************************************/
 



