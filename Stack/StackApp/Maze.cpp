#include <stdio.h>
#include <stdlib.h>
#include<time.h>

/*
	栈数据结构的应用——————>走迷宫
	2017年3月11日 10:33:34
*/

//记录通道块在迷宫矩阵当中的横、纵坐标 
struct Position {
	int x;
	int y;
};
//放入栈当中的通道块元素 
struct SElement {
	int ord;//通道块在路径上的序号
	Position p;//记录此通道块在矩阵当中的坐标位置 
	int di;//记录从此通道块走向下一道通道块的方向
};
//创建栈数据结构
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

#define TRUE  1
#define FALSE 0
#define  OK  1
#define  ERROR 0
#define  INFEASIBLE -1
#define OVERFLOW  -2
struct MazePathStack {
	SElement* base;
	SElement* top;
	int stacksize;
};

typedef int Status; //用于返回函数的状态

typedef int MazeType; //迷宫数据类型
#define MazeScale 10 //定义迷宫的大小
 /*  数字2表示墙，0代表可以通过的通道块 */
MazeType Maze[MazeScale][MazeScale] = {

	//迷宫样式
	{ 2,2,2,2,2,2,2,2,2,2 },
	{ 2,0,0,2,0,0,0,2,0,2 },
	{ 2,0,0,2,0,0,0,2,0,2 },
	{ 2,0,0,0,0,2,2,0,0,2 },
	{ 2,0,2,2,2,0,0,0,0,2 },
	{ 2,0,0,0,2,0,0,0,0,2 },
	{ 2,0,2,0,0,0,2,0,0,2 },
	{ 2,0,2,2,2,0,2,2,0,2 },
	{ 2,2,0,0,0,0,0,0,0,2 },
	{ 2,2,2,2,2,2,2,2,2,2 }
};

/* 随机生成迷宫数据 */
void makeMaze() {
	int i, j;
	srand((unsigned)time(NULL));
	for (i = 0; i<MazeScale; i++) {
		for (j = 0; j<MazeScale; j++) {
			if (i == 0 || i == MazeScale - 1 || j == 0 || j == MazeScale - 1) {
				Maze[i][j] = 2; //迷宫四周的墙
			}
			else {
				Maze[i][j] = rand() % 2;
			}
		}
	}
	Maze[1][1] = Maze[MazeScale - 2][MazeScale - 2] = 0; /* 起点和终点 */
}



//走迷宫函数接口
Status InitStack(MazePathStack* S); //创建一个空栈，如果创建成功则OK，否则就返回ERROR
Status IsStackEmpty(MazePathStack* S);//判断栈是否为空，如果是空的就返回TRUE，否则就返回FALSE 
Status GetTop(MazePathStack* S, SElement *e); // 获取栈顶元素,如果栈为空就返回ERROR 否则就返回OK
Status StackLength(MazePathStack* S);//获取栈的长度，并且通过程序返回 
Status Push(MazePathStack* S, SElement e); //插入元素e为新的栈顶元素，插入成功则返回OK，否则返回ERROR
Status Pop(MazePathStack* S, SElement* e);//弹出栈顶元素赋值给e弹出成功返回1，弹出失败返回0
Status Pass(Position p); //判断此通道块是否能通过，能通过返回TRUE ，否则返回FALSE
Position NextPosition(Position now, int direction);//按顺时针方向从东开始寻找矩阵当中某一个位置的临近位置 
void FootPrint(Position p, int step);//留下足迹 
void MarkPrint(Position p);//路径不可走的话就留下-1的标记 
Status MazePath(MazeType[MazeScale][MazeScale], Position start, Position end);
//若迷宫Maze中存在从入口start到出口end的通道，则求的一条存放在栈中的路径（从栈底到栈顶） ，并返回TURE ，否则返回FALSE

void main()
{
	//makeMaze(); //这行代码是随机生成迷宫矩阵的，但是经过我大约5100次的实验，生成的迷宫矩阵只有不到10次可以走通。
  Position start = { 1,1 };
	Position end = { 8,8 };
	if (MazePath(Maze, start, end))
	{
		printf("迷宫可走\n");
	}
	else
	{
		printf("迷宫走不出去呀!\n");
	}

	system("pause");
}

//走迷宫函数实现
Status InitStack(MazePathStack* S)
{
	S->base = (SElement *)malloc(STACK_INIT_SIZE * sizeof(SElement));
	if (!S->base)
		exit(OVERFLOW);
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}
Status IsStackEmpty(MazePathStack* S)
{
	if (S->base == S->top)
		return TRUE;
	else
		return FALSE;
}
Status GetTop(MazePathStack* S, SElement *e)
{
	if (S->base == S->top) 
		return ERROR;
	e = S->top - 1;
	return OK;
}
Status StackLength(MazePathStack* S)
{
	if (S->base == S->top)
		return ERROR;
	return S->top - S->base;
}
Status Push(MazePathStack* S, SElement e)
{
	if (S->top - S->base >= STACK_INIT_SIZE) // 栈满，追加存储空间
	{
		S->base = (SElement*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(SElement));
		if (!S->base)
			exit(OVERFLOW); //存储分配失败
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCREMENT;
	}

	*S->top = e;
	S->top++;
	return OK;

}
Status Pop(MazePathStack* S, SElement* e) 
{
	if (S->base == S->top)
		return ERROR;
	*e = *(S->top - 1);
	S->top--;

	return OK;
}
Status Pass(Position p)
{
	//只有路径所在位置为数字0的通道块才是可以通过的
	if (Maze[p.x][p.y] == 0)
		return TRUE;
	else
		return FALSE;
}
Position NextPosition(Position cur, int direction)
{
	Position Nextposi;
	int x = cur.x;
	int y = cur.y;
	switch (direction)
	{
		//东 
	case 1: {
		Nextposi.x = x;
		Nextposi.y = y + 1;
		break;
	}
			//南 
	case 2: {
		Nextposi.x = x + 1;
		Nextposi.y = y;
		break;
	}
			//西
	case 3: {
		Nextposi.x = x;
		Nextposi.y = y - 1;
		break;
	}
			//北
	case 4:
	{
		Nextposi.x = x - 1;
		Nextposi.y = y;
		break;
	}
	default:break;
	}
	return Nextposi;
}
void FootPrint(Position p, int step)
{
	Maze[p.x][p.y] = step;
}
void MarkPrint(Position p)
{
	Maze[p.x][p.y] = -1;
}
Status MazePath(MazeType[MazeScale][MazeScale], Position start, Position end)
{
	//打印迷宫矩阵
	for (int i = 0; i < MazeScale; i++)
	{
		for (int j = 0; j < MazeScale; j++)
		{
			printf("%d ", Maze[i][j]);
		}
		printf("\n");
	}

	//迷宫程序
	MazePathStack PathStack; //创建一个路径栈
	InitStack(&PathStack);
	Position curpos = start; //设置当前位置为入口位置
	int curStep = 1; //探索第一步
	do
	{
		if (Pass(curpos)) //如果当前位置可通
		{
			FootPrint(curpos, curStep); //留下足迹
			SElement e; //创建一个栈元素，存储可行路径的相关数据，将这个元素存储到栈中
			e.di = 1;//意味着下一个测试的方向是该位置的东方位的通道块
			e.ord = curStep; //该通道块在路径上的序号
			e.p.x = curpos.x;
			e.p.y = curpos.y; //该通道块在迷宫的坐标
			Push(&PathStack, e);//加入路径
			if (curpos.x == end.x && curpos.y == end.y)
			{
				//如果被压入的通道块是出口的通道块，就表示到达了迷宫的出口，此时退出循环
				//打印结果迷宫矩阵
				printf("\n迷宫路径为：\n");
				for (int i = 0; i < MazeScale; i++)
				{
					for (int j = 0; j < MazeScale; j++)
					{
						printf("%d\t", Maze[i][j]);
					}
					printf("\n");
				}
				return TRUE;
			}
			curpos = NextPosition(curpos, 1); //找到下一位置被测试的通道块，下一位置应该是当前位置的东邻
			curStep++; //探索下一步
		}
		else //如果当前通道块不能通过
		{
			if (!IsStackEmpty(&PathStack))
			{
				SElement e;
				Pop(&PathStack, &e);
				curStep--;
				while (e.di == 4 && !IsStackEmpty(&PathStack)) //留下不能通过的标记，并回退一步
				{
					MarkPrint(e.p);
					Pop(&PathStack, &e);
					curStep--;
				}
				//如果当前栈顶还有违背测试的路径就测试剩余的周围的路径
				if (e.di < 4)
				{
					curpos = NextPosition(e.p, e.di + 1);
					e.di++;
					curStep++;
					Push(&PathStack, e);
				}
			}
		}
	} while (!IsStackEmpty(&PathStack));
	printf("\n");
	return FALSE;


}


/*
在VS2015中的运行结果为：

2 2 2 2 2 2 2 2 2 2
2 0 0 2 0 0 0 2 0 2
2 0 0 2 0 0 0 2 0 2
2 0 0 0 0 2 2 0 0 2
2 0 2 2 2 0 0 0 0 2
2 0 0 0 2 0 0 0 0 2
2 0 2 0 0 0 2 0 0 2
2 0 2 2 2 0 2 2 0 2
2 2 0 0 0 0 0 0 0 2
2 2 2 2 2 2 2 2 2 2

迷宫路径为：
2       2       2       2       2       2       2       2       2       2
2       1       2       2       -1      -1      -1      2       0       2
2       0       3       2       -1      -1      -1      2       0       2
2       5       4       -1      -1      2       2       0       0       2
2       6       2       2       2       0       0       0       0       2
2       7       8       9       2       0       0       0       0       2
2       0       2       10      11      12      2       0       0       2
2       0       2       2       2       13      2       2       0       2
2       2       0       0       0       14      15      16      17      2
2       2       2       2       2       2       2       2       2       2
迷宫可走
请按任意键继续. . .


*/
