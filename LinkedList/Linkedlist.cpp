# include<stdio.h>
# include<stdlib.h>
/************************************************************************/
/*	线性表的链式存储结构
	2017年3月5日 10:51:07    
	Copyright © Castile All Rights Reserved.
*/
/************************************************************************/

typedef int ElemType ;  
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}Node;

typedef struct LNode * LinkList;

typedef int Status;
/*Status是函数的类型，其值是函数结果状态代码，如OK等*/

#define TRUE  1
#define FALSE 0
#define  OK  1
#define  ERROR 0
#define  INFEASIBLE -1
#define OVERFLOW  -2


//函数接口
Status InitList(LinkList &L); //初始化一个链表
void CreateList_Head(LinkList &L ,int n);//利用头插法创建一个链表
void CreateList_Tail(LinkList &L ,int n);//利用尾插法创建一个链表
Status GetElem(LinkList L ,int i,ElemType *e); // 获取第i个元素
Status ListInsert(LinkList &L,int i ,ElemType e); //插入元素
Status ListDelete(LinkList &L ,int i ,ElemType *e);//删除元素
Status DestroyList(LinkList &L); //销毁链表
Status ClearList(LinkList &L); //清空链表 
Status EmptyList(LinkList L); //判定链表是否为空
Status ListLength(LinkList L); //返回链表的长度（数据元素的个数） 
Status PriorElem(LinkList L,ElemType cur_e,ElemType *pre_e) ; ////若cur_e是L中的元素，且不是第一个，则返回其前驱元素  
Status NextElem(LinkList L,ElemType cur_e,ElemType *next_e); //若cur_e是L中的元素，且不是最后一个，返回其对应后继元素  
void ListTraverse(LinkList L); //遍历链表
void MergeList(LinkList &La,LinkList &Lb,LinkList &Lc); // 归并两个链表


/*测试代码*/
int main(void)
{
	ElemType val;
	LinkList L1 = NULL ;
	LinkList L2 = NULL;
	int n;
	LinkList La,Lb;
	LinkList Lc ;
	ElemType e;
	printf("利用头插法创建一个新链表：\n");
	printf("请输入你要插入结点的个数：");
	scanf("%d",&n);
	CreateList_Head(L1,n);
	printf("创建成功！\n");
	ListTraverse(L1);

	printf("利用尾插法创建一个新链表：\n");
	printf("请输入你要插入结点的个数：");
	scanf("%d",&n);
	CreateList_Tail(L2,n);
	printf("创建成功！\n");
	ListTraverse(L2);

	printf("请输入你要取出的结点位置：");
	scanf("%d",&n);
 	GetElem(L1,n,&val);
 	printf("取出的元素是：%d\n",val);

	printf("请输入你要插入的位置：");
	scanf("%d",&n);
	printf("请输入你要插入的结点数据：");
	scanf("%d",&e);
	ListInsert(L1,n,e);
	ListTraverse(L1);

	printf("链表的长度为：%d\n",ListLength(L1));

	printf("请输入你要删除结点的位置：");
	scanf("%d",&n);
	ListDelete(L1,n,&val);
	printf("删除的结点的数据元素是：%d\n",val);
	ListTraverse(L1);

	printf("请输入你要求结点的前驱：");
	scanf("%d",&n);
	if(PriorElem(L1,n,&val))
		printf("%d的前驱是：%d\n",n,val);

	printf("请输入你要求结点的后继：");
	scanf("%d",&n);
	if(NextElem(L1,n,&val))
		printf("%d的后继是：%d\n",n,val);
  DestroyList(L1);
	DestroyList(L2);

	return 0;
}


//函数实现
Status InitList(LinkList &L)
{
	L = (LinkList)malloc(sizeof(Node));
	if (L==NULL)
	{
		exit(OVERFLOW);
		
	}
	L->next = NULL;
	return OK;

}

void CreateList_Head(LinkList &L ,int n)
{
	LinkList p ;
	/*先建立一个带头结点的单链表*/
	L = (LinkList)malloc(sizeof(Node)); 
	L->next = NULL;
	for(int i=n ; i>0 ; i--)
	{
		p = (LinkList)malloc(sizeof(Node));//创建新结点
		printf("请输入结点的值：");
		scanf("%d",&p->data);
		p->next = L->next; //插入表头
		L->next=p;
	}
}

void CreateList_Tail(LinkList &L ,int n)//利用尾插法创建一个链表
{
	LinkList p,q; //q代表尾指针
	/*先初始化头结点*/
	L = (LinkList)malloc(sizeof(Node));
	L->next = NULL; 
	q = L;
	for(int i = 0; i < n ; i++)
	{
		p = (LinkList)malloc(sizeof(Node));//创建新结点
		printf("请输入新节点的值：");
		scanf("%d",&p->data);
		q->next = p;  //将表尾终端结点的指针指向新节点
		q=p; //将当前的新结点定义为表尾终端结点
	}
	q->next = NULL; //表示当前链表结束


	/************************************************************************/
	/* 注意L与q的关系，L是指整个单链表，而q是指向尾节点的指针变量，q会随着循环不断变化结点，
		而L是随着循环增长为一个多结点的链表
	*/
	/************************************************************************/
}

Status GetElem(LinkList L ,int i,ElemType *e)// 获取第i个元素
{
	LinkList p = L->next;
	int j = 1; //计数
	while (p && j<i) //p不为空且计数器j还没有等于i，循环继续
	{
		p = p->next;
		j++;
	}

	if(!p || j>i)  //第i个元素不存在
		return ERROR;
	*e = p->data;
	return OK;

}

Status ListInsert(LinkList &L,int i ,ElemType e)//在第i个数据元素之前插入一个新的元素
{
	LinkList p = L;
	LinkList pNew;
	int j =0;
	while(p && j<i-1) // 找到第i个结点的前驱结点
	{
		p = p->next;
		j++;
	}
	if(!p || j>i-1)
		return ERROR;
	
	pNew = (LinkList)malloc(sizeof(Node));
	pNew->data = e;
	pNew->next=p->next;
	p->next=pNew;//插入新结点
	return OK;
}

Status ListDelete(LinkList &L ,int i ,ElemType *e)//删除第i个元素
{
	LinkList p =L->next;
	int j=1;
	while(p && j<i-1)
	{
		p = p->next;
		j++;
	}
	if(!p || j>i-1)
		return ERROR;

	LinkList q = p->next;
	p->next =q->next;
	*e = q->data;
	free(q);

	return OK;
}

Status DestroyList(LinkList &L)//销毁链表
{
	LinkList p = L->next;
	LinkList q = NULL;
	while (p)
	{
		q = p;
		free(q);
		p= p->next;
	}
	free(L);
	return OK;
}

Status ClearList(LinkList &L)//清空链表 
{
	LinkList p = L->next;
	LinkList q = NULL;
	while (p)
	{
		q = p;
		free(q);
		p= p->next;
	}
	L->next = NULL;
	return OK;	
}

Status EmptyList(LinkList L) //判定链表是否为空
{
	if (L->next == NULL)
		return TRUE;
	else
		return FALSE;
}

Status ListLength(LinkList L) //求链表长度
{
	int length = 0;
	LinkList p = L->next;
	while(p)
	{
		length++;
		p = p ->next;
	}
	return length;
}

Status PriorElem(LinkList L,ElemType cur_e,ElemType *pre_e) ////若cur_e是L中的元素，且不是第一个，则返回其前驱元素  
{
	LinkList p =L;
	int pos =1;
	while (p->next && p->next->data!=cur_e)  
    {  
        p=p->next;  
        pos++;  
    }  
    if(pos!=1) //不是第一个结点  
    {  
        *pre_e = p->data;  
        return OK;  
    }  
    else //是第一个结点，则没有前驱结点
    {  
        printf("%d 是链表的第一个结点，没有前驱结点！\n", cur_e);  
        return ERROR;  
    }  

}
Status NextElem(LinkList L,ElemType cur_e,ElemType *next_e) //若cur_e是L中的元素，且不是最后一个，返回其对应后继元素
{
	int pos=1;  
    LinkList p=L->next;  
    while(p && p->data!=cur_e)  
    {     
        p=p->next;  
        pos++;  
    }  
    if(p->next!=NULL) //如果后继结点不为空
    {  
        *next_e = p->next->data; //保存后继结点的数据
        return OK;  
    }  
    else  //后继结点为空，说明已经到了最后一个元素，此时没有后继结点
    {  
        printf("%d是链表的最后一个结点，没有后继结点！\n", cur_e);  
        return ERROR;  
    }  

}
void ListTraverse(LinkList L) //遍历链表
{
	printf("\n********************链表内容**********************\n");
	LinkList p = L->next;
	while ( p )
	{
		printf( "%d\t", p->data );	//输出链表数据
		p = p->next;
	}
	printf("\n********************链表内容**********************\n");

}
void MergeList(LinkList &La,LinkList &Lb,LinkList &Lc)
{
	LinkList pa,pb,pc;
	Lc = pc =La; //将La的头结点赋给Lc
	pa = La->next;
	pb = Lb->next;
	while (pa && pb)
	{
		if(pa->data <= pb->data)
		{
			pc->next = pa;
			pc = pa;
			pa= pa->next;
		}
		else
		{
			pc->next = pb;
			pc = pb;
			pb = pb->next;
		}
	}
	pc->next = pa ? pa:pb; //插入剩余段
	free(Lb); //释放Lb的头结点
}


/* 在VC++6.0中的运行结果：

利用头插法创建一个新链表：
请输入你要插入结点的个数：5
请输入结点的值：1
请输入结点的值：2
请输入结点的值：3
请输入结点的值：4
请输入结点的值：5
创建成功！
  
********************链表内容**********************
5       4       3       2       1
********************链表内容**********************
利用尾插法创建一个新链表：
请输入你要插入结点的个数：5
请输入新节点的值：1
请输入新节点的值：2
请输入新节点的值：3
请输入新节点的值：4
请输入新节点的值：5
创建成功！
	
********************链表内容**********************
1       2       3       4       5
********************链表内容**********************
请输入你要取出的结点位置：2
取出的元素是：4
请输入你要插入的位置：3
请输入你要插入的结点数据：10
	  
********************链表内容**********************
5       4       10      3       2       1
********************链表内容**********************
链表的长度为：6
请输入你要删除结点的位置：5
删除的结点的数据元素是：2
		
********************链表内容**********************
5       4       10      3       1
********************链表内容**********************
请输入你要求结点的前驱：3
3的前驱是：10
请输入你要求结点的后继：4
4的后继是：10
Press any key to continue


*/
	
