#include<stdio.h>
#include<stdlib.h>
#include<assert.h> //使用断言函数assert

/************************************************************************/
/* 栈的运用 ------->表达式求值 ：输入算术表达式，运算符限定为"+ - * / ( )"，操作数限定为0-9。求表达式的值    
	2017年3月12日 08:55:22    
  利用算符优先算法   

  假设运算符a和b相继出现，则+、-、*和/为a时的优先性均低于’(’但高于’)’；
  为了满足从左算到右的规则，当a=b时，令a>b，’#’是表达式的结束符。为了算法简洁，
  在表达式的最左边也虚设一个’#’构成整个表达式的一对括号。
  ‘(’=’)’，表示当左右括号相遇时，括号内的运算已经完成。同理’#’=’#’表示整个表达式求值完毕。
  在处理表达式前，首先设置两个栈：操作数栈(OPND)：用于寄存操作数和运算结果；运算符栈(OPTR)：用于寄存运算符。
  算法的基本思想：
  1)首先置操作数栈为空栈，表达式起始符’#’为运算符栈的栈底元素；
  2)依次读入表达式中每个字符，若是操作数则进OPND栈，若是运算符则和OPTR栈的栈顶运算符比较优先权后做相应操作，
    直至整个表达式求值完毕(即OPTR栈的栈顶元素和当前读入的字符均为’#’)。                                                         */
/************************************************************************/

typedef int Status;
//创建栈数据结构
#define STACK_INIT_SIZE 10
#define STACKINCREMENT 5
#define TRUE  1
#define FALSE 0
#define  OK  1
#define  ERROR 0
#define  INFEASIBLE -1
#define OVERFLOW  -2

typedef union{
	int number; //数字
	char character; //运算符
}ElemType;


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
ElemType* GetTop(Stack *s)
{
    if (s->top == s->base)
        return NULL;
    return (s->top - 1);
}
Status StackLength(Stack* S)
{
	if (S->base == S->top)
		return ERROR;
	return S->top - S->base;
}
Status Push(Stack* S, ElemType *e)
{
	if (S->top - S->base >= S->stacksize) // 栈满，追加存储空间
	{
		S->base = (ElemType*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(ElemType));
		if (!S->base)
			exit(OVERFLOW); //存储分配失败
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCREMENT;
	}
	
	*(S->top) = *e;
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

//判断字符c是否在字符串ch中
Status in(char c,char* ch)
{
	
	while(*ch && (*ch) != c)
		ch++;
	if(*ch)
		return TRUE;
	return FALSE;
}

//比较运算符的优先级
char Priority(char a, char b)
{
	switch (a)
    {
    case '+':
        if (in(b, "+-)#"))
            return '>';
        else if (in(b, "*/("))
            return '<';
        break;
    case '-':
        if (in(b, "+-)#"))
            return '>';
        else if (in(b, "*/("))
            return '<';
        break;
    case '*':
        if (in(b, "+-*/)#"))
            return '>';
        else if (in(b, "("))
            return '<';
        break;
    case '/':
        if (in(b, "+-*/)#"))
            return '>';
        else if (in(b, "("))
            return '<';
        break;
    case '(':
        if (in(b, "+-*/("))
            return '<';
        else if (in(b, ")"))
            return '=';
        break;
    case ')':
        if (in(b, "+-*/)#"))
            return '>';
        break;
    case '#':
        if (in(b, "+-*/("))
            return '<';
        else if (in(b, "#"))
            return '=';
        break;
    }

}

//计算
Status Calculate(int a ,char opera ,int b)
{
	switch (opera)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        assert(b != 0);
        return a / b;
    }
	
}

void main()
{
	ElemType ch,ch_prior,temp;  //temp用于进栈和出栈的临时变量
    ElemType left, right;
    ElemType result;
    Stack OPTR,OPND;    //运算符栈和操作数栈
    char *num = "0123456789";
    char *oper = "+-*/()#";
	
    InitStack(&OPTR);
    temp.character = '#';
    Push(&OPTR, &temp);
	
    InitStack(&OPND);
	
    ch.character = getchar();
	
    //循环停止条件：栈顶元素为'#'，并且当前待进栈元素也为'#'。表明#号中间的表达式计算完毕。
    while (!(GetTop(&OPTR)->character == '#' && ch.character == '#'))
    {
        //若当前输入字符为数字，进操作数栈。继续读取下一个字符。
        if (in(ch.character, num))
        {
            temp.number = ch.character - '0';
            Push(&OPND, &temp);
            ch.character = getchar();
        }
        else if (in(ch.character, oper))//如果当前字符为运算符，比较栈顶运算符与当前运算符的优先级。
        {
            switch (Priority(GetTop(&OPTR)->character, ch.character))
            {
                //若栈顶运算符优先级较高，则先计算中间值。
                //注意，此时并未将当前运算符进栈，也没有读入下一个字符。
                //而是在下一次while循环中继续将当前运算符和栈顶运算符做比较。
            case '>':
                Pop(&OPND, &right);
                Pop(&OPND, &left);
                Pop(&OPTR, &ch_prior);
                temp.number = Calculate(left.number, ch_prior.character, right.number);
                Push(&OPND,&temp);
                break;
				
                //若当前运算符优先级较高，将运算符进栈。继续读取下一个字符。
            case '<':
                Push(&OPTR, &ch);
                ch.character = getchar();
                break;
				
                //两个运算符优先级相同表明，栈顶运算符为'('，当前运算符为')'。此时将栈顶元素弹出。继续读取下一个字符。
            case '=':
                Pop(&OPTR, &ch_prior);
                ch.character = getchar();
                break;
            }
        }
    }
    //此时，操作数栈只有一个元素，即为计算结果。
    Pop(&OPND, &result);
    printf("结果为：%d\n", result.number);
    
}

/*
	运行结果：
	2*(3+8)#
	结果为：22
	Press any key to continue

*/

