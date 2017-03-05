#include<stdio.h>
#include<stdlib.h>
/************************************************************************/
/*双向链表存储结构(Double linked list)是在单链表的每个结点中，再设置一个指向其前驱结点的指针域  
    Copyright © Castile All Rights Reserved.
    2017年3月5日 22:34:02                                                                   */
/************************************************************************/
typedef int ElemType ;  
typedef int Status;

typedef struct DulNode
{
	ElemType data;
	struct DulNode *prior; /*直接前驱指针*/
	struct DulNode *next;  /*直接后继指针*/
}DulNode , *DuLinkList;


#define TRUE  1
#define FALSE 0
#define  OK  1
#define  ERROR 0
#define  INFEASIBLE -1
#define OVERFLOW  -2

/************************************************************************/
/* 双向链表的一些性质：
	双向链表也是循环链表；
	对于链表中的某一个节点p，它的后继的前驱是它自己，它的前驱的后继也是它自己。即：
	p->next->prior = p = p->prior->next                                  */
/************************************************************************/

//双向链表的一些操作，如查找元素GetElem，求长度Listlength等涉及一个方向的指针时和单链表的操作相同
//插入和删除操作有点不同，需要更改两个指针变量
Status InitDulist(DuLinkList &L)
{
	L = (DuLinkList)malloc(sizeof(DulNode));
	if(L==NULL)
	{
		printf("动态内存分配失败！");
		exit(OVERFLOW);
	}
	L->next = L;
	L->prior = L;

	return OK;
}

Status GetElem_Du(DuLinkList L , int i ,ElemType *e)
{
	DuLinkList p = L->next;
	int j=1;
	while(p!=L && j<i)
	{
		p = p->next;
		j++;
	}
	if(p == L || j>i)
		return ERROR;
	*e = p->data;

	return OK;
}

Status ListInsert_Dul(DuLinkList &L,int i,ElemType e)
{
	//在带头结点的双向循环链表L中第i个位置之前插入元素e
	DuLinkList p = L;
	DuLinkList pNew;
	int j =0;
	while(p && j<i-1) // 找到第i个结点的前驱结点
	{
		p = p->next;
		j++;
	}
	if(!p || j>i-1)
		return ERROR;
	
	pNew = (DuLinkList)malloc(sizeof(DulNode));
	pNew->data = e;
	pNew->prior = p;
	pNew->next = p ->next;
	p->next->prior = pNew;
	p->next = pNew;
	return OK;
}

Status ListDelete_Dul(DuLinkList &L ,int i ,ElemType *e)
{
	DuLinkList p = L->next;
	int j=0;
	while(p && j<i-1)
	{
		p = p->next;
		j++;
	}
	if(!p || j>i-1)
		return ERROR;
	*e = p->data;
	p->prior->next = p->next;
	p->next->prior = p->prior;
	free(p);
	return OK;
}


void ListTraverse(DuLinkList L) //遍历链表
{
	printf("\n********************链表内容**********************\n");
	DuLinkList p = L->next;
	while ( p != L )
	{
		printf( "%d\t", p->data );	//输出链表数据
		p = p->next;
	}
	printf("\n********************链表内容**********************\n");
	
}
void main()
{
	DuLinkList L ;
	ElemType e;
	int n;
	InitDulist(L);
	ListInsert_Dul(L,1,2);
	ListInsert_Dul(L,2,3);
	ListInsert_Dul(L,3,5);
	ListInsert_Dul(L,3,6);
	ListInsert_Dul(L,3,9);

	ListTraverse(L);
	printf("请输入你要删除的位置：");
	scanf("%d",&n);
	ListDelete_Dul(L,n,&e);
	printf("你删除的数据元素是：%d",e);
	ListTraverse(L);
}

/************************************************************************/
/* 在VC++6.0中运行结果：

  ********************链表内容**********************
  2       3       9       6       5
  ********************链表内容**********************
  请输入你要删除的位置：4
  你删除的数据元素是：6
  ********************链表内容**********************
  2       3       9       5
  ********************链表内容**********************
Press any key to continue                                                                   */
/************************************************************************/
