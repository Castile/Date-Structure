#include<stdio.h>
#include<stdlib.h>

/************************************************************************/
/* 栈的应用 ----->括号匹配
	2017年3月11日 22:54:38

假设表达式中含有3种括号：(),[],{}，其嵌套的顺序随意。检验括号是否匹配。
基本思想：在算法中设置一个栈，每读入一个括号，若是右括号，则或者与栈顶匹配的左括号相互消解，
或者是不合法的情况；若是左括号，则直接压入栈中。若括号匹配，在算法的开始和结束时，栈都应该是空的。                                                                     */
/************************************************************************/


typedef int Status;
typedef char ElemType;
//创建栈数据结构
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

#define TRUE  1
#define FALSE 0
#define  OK  1
#define  ERROR 0
#define  INFEASIBLE -1
#define OVERFLOW  -2

struct Stack {
	ElemType* base;
	ElemType* top;
	int stacksize;
};
 
/*栈的操作*/
Status InitStack(Stack* S)
{
	S->base = (ElemType *)malloc(STACK_INIT_SIZE * sizeof(ElemType));
	if (!S->base)
		exit(OVERFLOW);
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}
Status IsStackEmpty(Stack* S)
{
	if (S->base == S->top)
		return TRUE;
	else
		return FALSE;
}
Status GetTop(Stack* S, ElemType *e)
{
	if (S->base == S->top) 
		return ERROR;
	*e = *(S->top - 1);
	return OK;
}
Status StackLength(Stack* S)
{
	if (S->base == S->top)
		return ERROR;
	return S->top - S->base;
}
Status Push(Stack* S, ElemType e)
{
	if (S->top - S->base >= S->stacksize) // 栈满，追加存储空间
	{
		S->base = (ElemType*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(ElemType));
		if (!S->base)
			exit(OVERFLOW); //存储分配失败
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCREMENT;
	}
	
	*S->top = e;
	S->top++;
	return OK;
	
}
Status Pop(Stack* S, ElemType* e) 
{
	if (S->base == S->top)
		return ERROR;
	*e = *(S->top - 1);
	S->top--;
	
	return OK;
}

//判断左括号和右括号是否相匹配
Status match(ElemType a,ElemType b)
{
	if(a == '(' && b == ')')
		return TRUE;
	if(a == '{' && b == '}')
		return TRUE;
	if(a == '[' && b == ']')
		return TRUE;

	return FALSE;

}

//判断字符e是否位于字符串ch中
Status In(ElemType e,ElemType* ch)
{

	while(*ch && (*ch) != e)
		ch++;
	if(*ch)
		return TRUE;
	return FALSE;
}

void main()
{
	Stack S;
	ElemType ch,ch_prior;
	ElemType* Left = "([{";
	ElemType* Right = ")]}";
	bool flag = true;
	InitStack(&S);
	printf("请输入括号：\n");
	while ((ch = getchar()) == '\n')
	{
		if(In(ch ,Left))
		{//如果字符为左括号，则进栈
			Push(&S,ch);
			
		}
		else if(In(ch,Right)) //如果字符为右括号，判断栈顶元素是否存在并且与当前字符匹配
		{
			if(GetTop(&S,&ch_prior) && match(ch_prior,ch))
			{
				Pop(&S,&ch);
				
			}
			else
			{
				flag = false;
				break;
			}
		}
	}
	
	/*
		也可以用一个switch语句来进行判断
		while(str[i] != '\0')  
		{  
		switch(str[i])  
		{  
		case '(':  
		Push(&S,str[i]);  
		break;  
		case '[':  
		Push(&S,str[i]);  
		break;  
		case '{':  
		Push(&S,str[i]);  
		break;  
		case ')':  
		{  
		Pop(&S,&e);  
		if(e != '(')   
		flag=1;  
		}  
		break;  
		case ']':  
		{  
		Pop(&S,&e);  
		if(e!='[')  
		flag=1;  
		}  
		break;  
		case '}':  
		{  
		Pop(&S,&e);  
		if(e!='{')  
		flag=1;  
		}  
		break;  
		default:  
		break;  
		}  
		if(flag)  
		break;  
		i++;  
    }  
	
	
	*/

	
	if(flag)
	{
		puts("括号匹配正确！\n");
	}
	else
	{
		puts("括号匹配错误！\n");
	}
	
}
