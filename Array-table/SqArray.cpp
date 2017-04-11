#include<stdarg.h> /* 标准头文件，提供宏va_start，va_arg和va_end， */
				   /* 用于存取变长参数表 */
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
/************************************************************************/
/*	stdarg.h是C语言中C标准函数库的头文件，stdarg是由standard（标准） arguments（参数）简化而来，
	主要目的为让函数能够接收可变参数。     

  stdarg.h数据类型：

  va_list	用来保存宏va_arg与宏va_end所需信息

  stdarg.h宏：

  va_start	使va_list指向起始的参数

  va_arg	检索参数

  va_end	释放va_list

  va_copy	拷贝va_list的内容
                                                            */
/************************************************************************/


/*
	数组的顺序存储表示
	2017年4月10日 20:46:50

*/
#define MAX_ARRAY_DIM 8 /* 假设数组维数的最大值为8 */
typedef int ElemType;
typedef struct
{
	ElemType *base; /* 数组元素基址，由InitArray分配 */
	int dim; /* 数组维数 */
	int *bounds; /* 数组维界基址，由InitArray分配 */
	int *constants; /* 数组映象函数常量基址，由InitArray分配 */
}Array;

typedef int Status; //函数返回的状态

#define TRUE		1
#define FALSE		0
#define OK			1
#define ERROR		0
#define INFEASIBLE	-1

/*函数操作*/
Status InitArray(Array *A,int dim,...)
{ /* 若维数dim和各维长度合法，则构造相应的数组A，并返回OK */
	int elemtotal=1,i; /* elemtotal是元素总值 */
	va_list ap;
	if(dim<1||dim>MAX_ARRAY_DIM)
		return ERROR;
	(*A).dim=dim;
	(*A).bounds=(int *)malloc(dim*sizeof(int));
	if(!(*A).bounds)
		exit(OVERFLOW);
	va_start(ap,dim);
	for(i=0;i<dim;++i)
	{
		(*A).bounds[i]=va_arg(ap,int);
		if((*A).bounds[i]<0)
			return UNDERFLOW; /* 在math.h中定义为4 */
		elemtotal*=(*A).bounds[i];
	}
	va_end(ap);
	(*A).base=(ElemType *)malloc(elemtotal*sizeof(ElemType));
	if(!(*A).base)
		exit(OVERFLOW);
	(*A).constants=(int *)malloc(dim*sizeof(int));
	if(!(*A).constants)
		exit(OVERFLOW);
	(*A).constants[dim-1]=1;
	for(i=dim-2;i>=0;--i)
		(*A).constants[i]=(*A).bounds[i+1]*(*A).constants[i+1];
	return OK;
 }

Status DestroyArray(Array *A)
{ /* 销毁数组A */
	if((*A).base)
	{
		free((*A).base);
		(*A).base=NULL;
	}
	else
		return ERROR;
	if((*A).bounds)
	{
		free((*A).bounds);
		(*A).bounds=NULL;
	}
	else
		return ERROR;
	if((*A).constants)
	{
		free((*A).constants);
		(*A).constants=NULL;
	}
	else
		return ERROR;
	return OK;
}

Status Locate(Array A,va_list ap,int *off) /* Value()、Assign()调用此函数 */
{ /* 若ap指示的各下标值合法，则求出该元素在A中的相对地址off */
	int i,ind;
	*off=0;
	for(i=0;i<A.dim;i++)
	{
		ind=va_arg(ap,int);
		if(ind<0||ind>=A.bounds[i])
			return OVERFLOW;
		*off+=A.constants[i]*ind;
	}
	return OK;
}

Status Value(ElemType *e,Array A,...) /* 在VC++中，...之前的形参不能是引用类型 */
{ /* ...依次为各维的下标值，若各下标合法，则e被赋值为A的相应的元素值 */
	va_list ap;
	Status result;
	int off;
	va_start(ap,A);
	if((result=Locate(A,ap,&off))==OVERFLOW) /* 调用Locate() */
		return result;
	*e=*(A.base+off);
	return OK;
}

Status Assign(Array *A,ElemType e,...)
{ /* ...依次为各维的下标值，若各下标合法，则将e的值赋给A的指定的元素 */
	va_list ap;
	Status result;
	int off;
	va_start(ap,e);
	if((result=Locate(*A,ap,&off))==OVERFLOW) /* 调用Locate() */
		return result;
	*((*A).base+off)=e;
	return OK;
}

void main()
{
	Array A;
	int i,j,k,*p,dim=3,bound1=3,bound2=4,bound3=2; /* a[3][4][2]数组 */
	ElemType e,*p1;
	InitArray(&A,dim,bound1,bound2,bound3); /* 构造3＊4＊2的3维数组A */
	p=A.bounds;
	printf("A.bounds=");
	for(i=0;i<dim;i++) /* 顺序输出A.bounds */
		printf("%d ",*(p+i));
	p=A.constants;
	printf("\nA.constants=");
	for(i=0;i<dim;i++) /* 顺序输出A.constants */
		printf("%d ",*(p+i));
	printf("\n%d页%d行%d列矩阵元素如下:\n",bound1,bound2,bound3);
	for(i=0;i<bound1;i++)
	{
		for(j=0;j<bound2;j++)
		{
			for(k=0;k<bound3;k++)
			{
				Assign(&A,i*100+j*10+k,i,j,k); /* 将i*100+j*10+k赋值给A[i][j][k] */
				Value(&e,A,i,j,k); /* 将A[i][j][k]的值赋给e */
				printf("A[%d][%d][%d]=%2d ",i,j,k,e); /* 输出A[i][j][k] */
			}
			printf("\n");
		}
		printf("\n");
	}
	p1=A.base;
	printf("A.base=\n");
	for(i=0;i<bound1*bound2*bound3;i++) /* 顺序输出A.base */
	{
		printf("%4d",*(p1+i));
		if(i%(bound2*bound3)==bound2*bound3-1)
			printf("\n");
	}
	DestroyArray(&A);
}

/************************************************************************/
/* 在VC++6.0中的运行结果为：

A.bounds=3 4 2
A.constants=8 2 1
3页4行2列矩阵元素如下:
A[0][0][0]= 0 A[0][0][1]= 1
A[0][1][0]=10 A[0][1][1]=11
A[0][2][0]=20 A[0][2][1]=21
A[0][3][0]=30 A[0][3][1]=31

A[1][0][0]=100 A[1][0][1]=101
A[1][1][0]=110 A[1][1][1]=111
A[1][2][0]=120 A[1][2][1]=121
A[1][3][0]=130 A[1][3][1]=131

A[2][0][0]=200 A[2][0][1]=201
A[2][1][0]=210 A[2][1][1]=211
A[2][2][0]=220 A[2][2][1]=221
A[2][3][0]=230 A[2][3][1]=231

A.base=
0   1  10  11  20  21  30  31
100 101 110 111 120 121 130 131
200 201 210 211 220 221 230 231
Press any key to continue
                                                                     */
/************************************************************************/
