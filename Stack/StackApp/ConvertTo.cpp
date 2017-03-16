#include <stdio.h>
#include <stdlib.h>
#define N 50 
/*
	栈的运用--->进制转换  10—>2   10--->8   10---->16
	2017年3月16日 21:47:16
*/
typedef struct Mystack
{
	int top;//栈顶
	int data[N];//存放数据

} Stack;

Stack stack = {-1,{0}}; //栈的初始化
int isempty(Stack *);//1 表示为空， 0 表示不为空
void setempty(Stack *);//设置栈为空
int push(Stack *stack,int num);//压栈
int pop(Stack *);//出栈
void tenTotwo(int num); //十进制转二进制
void tentoOctal(int num);//十进制转八进制
void tentoHex(int num);//十进制转十六进制

int main(void)
{

	Stack stack = {-1,{0}}; //栈的初始化
	tenTotwo(1000);
	tentoOctal(1000);
	tentoHex(1000);
	return 0;
}

int isempty(Stack *stack)
{
	if(stack->top==-1)
		return 1;
	else
		return 0;
}

void setempty(Stack *stack)
{
	stack->top=-1;
	
}

int push(Stack *stack,int num)
{
	if(stack->top == N-1)
	{
		return 0;//失败
	}
	else
	{
		stack->top+=1;
		stack->data[stack->top]=num;
		return 1;
	}
}

int pop(Stack *stack)
{
	if(isempty(stack))
	{
		return -1;//栈为空
	}
	else
	{
		stack->top-=1;
	    return stack->data[stack->top+1];//返回弹出的数据

	}
}


//十进制转二进制
void tenTotwo(int num)
{
	while(num)
	{
		push(&stack,num%2);
		num/=2;
	}

	while(!isempty(&stack))
	{
		printf("%d",pop(&stack));
	}
	printf("\n");

}
//十进制转八进制
void tentoOctal(int num)
{

	while(num)
	{
		push(&stack,num%8);
		num/=8;
	}
	
	while(!isempty(&stack))
	{
		printf("%d",pop(&stack));
	}
	printf("\n");
}

//十进制转十六进制
void tentoHex(int num)
{
	while(num)
	{
		push(&stack,num%16);
		num/=16;
	}
	
	while(!isempty(&stack))
	{
		int n = pop(&stack);
		switch(n)
		{
		case 10:
			printf("%c",'A');
			break;
		case 11:
			printf("%c",'B');
			break;
		case 12:
			printf("%c",'C');
			break;
		case 13:
			printf("%c",'D');
			break;
		case 14:
			printf("%c",'E');
			break;
		case 15:
			printf("%c",'F');
			break;
		default:
			printf("%d",n);

			
		}
	}
	printf("\n");

}
