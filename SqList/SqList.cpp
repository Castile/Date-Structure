# include <stdio.h>
# include <stdlib.h>
# include <malloc.h>


/************************************************************************/
/* 2017年2月28日 21:51:24  Castile All Rights Reserved
《数据结构》 严蔚敏版 第二章：线性表连续存储算法的算法实现
运行环境：VC6.0                                                                     */
/************************************************************************/
#define TRUE  1
#define FALSE 0
#define  OK  1
#define  ERROR 0
#define  INFEASIBLE -1
#define OVERFLOW  -2

#define LISTINITSIZE 20 //存储空间初始分配量
#define LISTINCREMENT 10 //线性表存储空间的分配增量

typedef int Status;
/*Status是函数的类型，其值是函数结果状态代码，如OK等*/
typedef int ElemType; //ElemType 类型根据实际情况而定，这里假设为int

typedef struct
{
	ElemType *elem; //存储空间基址
	int length; //当前长度
	int listsize; //当前分配的存储容量（以sizeof(ElemType)为单位）
}Sqlist;

/*
	在上述定义中，数组指针elem指示线性表的基地址，length表示线性表的当前长度。
	listsize指示顺序表当前分配的存储空间大小，一旦因插入元素而空间不足时，可进行再分配，即为顺序表增加一个大小为存储LISTINCREMENT个数据元素空间
*/



/* 函数接口 */


Status InitList(Sqlist *L); //初始化操作，建立一个空的线性表
Status DestroyList(Sqlist *L);
/*
	初始条件：线性表L已存在
	操作结果：销毁线性表L
*/
Status ListEmpty(Sqlist L);
/*
	初始条件：线性表L已存在
	操作结果：将L重置为空表
*/
Status ListLength(Sqlist L);
/*
	初始条件：线性表L已存在
	操作结果：返回L中数据元素个数
*/
Status GetElem(Sqlist L ,int i,ElemType *e);
/*
	初始条件：线性表L已存在,1<= i <=ListLength(L)
	操作结果：用e返回L中第i个数据元素的值

*/
int LocateElem(Sqlist L ,ElemType e); 
/* 在线性表L中查找与给定元素e相等的元素，如果查找成功，返回该元素在表中序号表示成功，否则返回0表示失败*/

Status ListInsert(Sqlist *L,int i,ElemType e);
/*
	初始条件：线性表L已存在,1<= i <=ListLength(L)+1
	操作结果：在L中第i个位置之前插入新的数据元素e，L的长度加1
*/
Status ListDelete(Sqlist *L,int i,ElemType* e);
/*
	初始条件：线性表L已存在,1<= i <=ListLength(L)
	操作结果：删除L的第i个数据元素，并用e返回其值，L的长度减1
*/
Status ListTraverse(Sqlist L);//遍历线性表L

void unionList(Sqlist *La,Sqlist Lb);//将所有在线性表Lb中但不在La中的数据元素插入到La中
void MergeList_Sq(Sqlist La ,Sqlist Lb ,Sqlist *Lc);//使用数组下标法
void MergeList_Point(Sqlist La ,Sqlist Lb ,Sqlist *Lc);//使用指针法
/*
	已知线性表La和Lb中的数据元素按值非递减排列。
	归并La和Lb得到新的线性表Lc，Lc的数据元素也按值非递减排列
*/

int main(void)
{
	/* test code*/
	
	Sqlist List;
	ElemType e;
	InitList(&List);
	ListInsert(&List,1,2);
	ListInsert(&List,2,4);
	ListInsert(&List,3,6);
	ListInsert(&List,4,8);
	ListTraverse(List);
	ListInsert(&List,2,10);
	ListTraverse(List);
	printf("线性表的长度是：%d\n",	ListLength(List));
	ListDelete(&List,2,&e);
	ListTraverse(List);
	printf("删除的元素是：%d\n",e);
	


	//测试线性表的合并功能
	Sqlist La,Lb,Lc;

	/*一定要先进行线性表的初始化*/
	InitList(&La);
	InitList(&Lb);
	ListInsert(&La,1,3);
	ListInsert(&La,2,5);
	ListInsert(&La,3,8);
	ListInsert(&La,4,11);
	ListInsert(&Lb,1,2);
	ListInsert(&Lb,2,6);
	ListInsert(&Lb,3,8);
	ListInsert(&Lb,4,9);
	ListInsert(&Lb,5,11);
	ListInsert(&Lb,6,15);
	ListInsert(&Lb,7,20);
	printf("La中的数据元素：");
	ListTraverse(La);
	printf("Lb中的数据元素：");
	ListTraverse(Lb);
	//unionList(&La,Lb);
	//printf("合并后：\n");
	//ListTraverse(La);


	//测试线性表的归并功能
 	
	MergeList_Point(La,Lb,&Lc);
	//MergeList_Sq(La,Lb,&Lc);
 	printf("归并后：");
  	ListTraverse(Lc);


	
	return 0;
}

Status InitList(Sqlist *L)
{
	//构造一个空的线性表
	L->elem = (ElemType *)malloc(LISTINITSIZE*sizeof(ElemType));
	if(!L->elem)
	{
		printf("动态内存分配失败！\n");
		exit(OVERFLOW);
	}
	L->length=0;//空表长度为0
	L->listsize=LISTINITSIZE; //初始存储容量
	return OK;
}

Status DestroyList(Sqlist *L)
{
	if(ListEmpty(*L)) //链表为空
		return ERROR;

	InitList(L);

	return OK;
}

Status ListEmpty(Sqlist L)
{
	if(L.length==0)
		return TRUE;
	else
		return FALSE;
}

Status ListLength(Sqlist L)
{
	if(L.length!=0)
		return L.length;
	else	//若L为空表
		return ERROR;
}

Status GetElem(Sqlist L ,int i,ElemType *e)
{
	if(L.length==0 || i<1 || i>L.length)//若为空表或者i值不合法，返回ERROR
		return ERROR;
	*e = L.elem[i-1];
	return OK;
}


Status ListInsert(Sqlist *L,int i,ElemType e)
{
	int k;
	if( L->length==LISTINITSIZE ) /*顺序线性表已满*/
		return ERROR;
	if(i<1 || i>L->length+1)/*当i不在范围内时*/
		return ERROR;
	if(i<=L->length) /* 当插入的位置不在表尾*/
	{
		
		for(k=L->length-1 ; k>=i-1 ; k--) //将要插入位置后的数据元素先后移动一位
			L->elem[k+1] = L->elem[k];
		
	}
	L->elem[i-1] = e; //将新元素插入
	L->length++;

	/*

	另一种方式：指针法	
		ElemType* q = &(L.elem[i-1]); //q为插入的位置
		for(ElemType* p=&(L.elem[L.length-1]) ; p>=q ; --p)
				*(p+1) = *p;  //插入位置及之后的元素后移	
		*q = e; //将e插入
		++L->length ; //表长加1
	
*/

	return OK;

}


Status ListDelete(Sqlist *L,int i,ElemType* e)
{
	int k;
	if(L->length == 0) //若线性表为空
		return ERROR;
	if(i<i || i>L->length) //删除位置不正确
		return ERROR;
	*e = L->elem[i-1];
	if(i<L->length) //如果删除的不是最后位置
	{
		for(k=i ; k<L->length ; k++)
			L->elem[k-1] = L->elem[k];
	}
	L->length--;
	/*
		指针法：
			ElemType* p = &(L.elem[i-1]); //p为被删除元素的位置
			e = *p; //被删除的元素赋值给e
			ElemType* q = L.elem+L.length-1; //表尾元素的位置
			for(++p ; p<=q ; ++p)
				*(p-1) = *p;  //被删除元素之后的元素向左移动
			--L.length;
	*/

	return OK;

}

int LocateElem(Sqlist L ,ElemType e)
{
	int i = 1; //i的初值为第一个元素的位序
	ElemType* p = L.elem; //p的初值为第一个元素的存储位置
	while(i<=L.length &&  p[i-1]!=e)
		i++;
	
	if(i<=L.length)
		return i;
	else
		return 0;
	
}

Status ListTraverse(Sqlist L)
{
	int i;
	if(ListEmpty(L))
		return ERROR;
	for (i=0 ; i<L.length ; ++i)
	{
		printf("%d	",L.elem[i]);
	}
	printf("\n");
	return OK;

}

void unionList(Sqlist *La,Sqlist Lb)
{
	//先求线性表的长度
	int La_len = ListLength(*La);
	int Lb_len = ListLength(Lb);
	int i;
	ElemType e;
	for (i=1 ; i<=Lb_len ; i++)
	{
		GetElem(Lb,i,&e); //取Lb中第i个数据元素赋值给e
		if(!LocateElem(*La,e)) /* La中不存在和e相同的数据元素*/
			ListInsert(La , ++La_len ,e);//插入
	}

}


/*此算法有错误！笔者还未解决！------调用此函数会造成死循环，原因不详*/
void MergeList_Sq(Sqlist La ,Sqlist Lb ,Sqlist *Lc)
{
	int i=1;
	int j=1;
	InitList(Lc);
	ElemType ai;
	ElemType bj; //用来存放在La和Lb中取出来的数据元素
	int k=0;
	int La_len=ListLength(La);
	int Lb_len=ListLength(Lb);
	while(i <= La_len && j <= Lb_len) //La和Lb非空
	{
		GetElem(La,i,&ai);
		
		GetElem(Lb,j,&bj);
		
		if(ai<=bj)
		{
			ListInsert(Lc,++k,ai);
			i++;
		}
		else
		{
			ListInsert(Lc,++k,bj);
			j++;
		}

	}
	/*接下来处理超长的部分*/
	while(i<=La_len)
	{
		GetElem(La,i++,&ai);
		ListInsert(Lc,++k,ai);
	}
	while(i<=Lb_len)
	{
		GetElem(Lb,j++,&bj);
		ListInsert(Lc,++k,bj);
	}
}

//归并函数
void MergeList_Point(Sqlist La ,Sqlist Lb ,Sqlist *Lc)
{
    ElemType *pa=La.elem; 
    ElemType *pb=Lb.elem;
    Lc->listsize=Lc->length=La.length+Lb.length;
    ElemType *pc=Lc->elem=(ElemType*)malloc(Lc->listsize*sizeof(ElemType));//从右向左结合方向
    if(!Lc->elem)
    {
        exit(OVERFLOW);
    }
    ElemType *pa_last=La.elem+La.length-1;
    ElemType *pb_last=Lb.elem+Lb.length-1;
    while(pa<=pa_last&&pb<=pb_last)//归并
    {
		if(*pa<=*pb) 
		{
			*pc++=*pa++;
		}
		else
		{
			*pc++=*pb++;
		}
    }
    while(pa<=pa_last)//插入La中剩余的元素
    {
        *pc++=*pa++;
    }
    while(pb<=pb_last)//插入Lb中剩余的元素
    {
        *pc++=*pb++;
    }
}

/************************************************************************/
/* 在vc++6.0中的运行结果为：
2       4       6       8
2       10      4       6       8
线性表的长度是：5
2       4       6       8
删除的元素是：10
La中的数据元素：3       5       8       11
Lb中的数据元素：2       6       8       9       11      15      20
归并后：2       3       5       6       8       8       9       11      11      15      20
Press any key to continue
                                                                     */
/************************************************************************/
  

