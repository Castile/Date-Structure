#include<stdio.h>
#include<stdlib.h>

/* 稀疏矩阵的三元组顺序表存储表示 */
#define MAXSIZE 100 /* 非零元个数的最大值 */
typedef int ElemType;
typedef struct
{
	int i,j; /* 行下标,列下标 */
	ElemType e; /* 非零元素值 */
}Triple;
typedef struct
{
	Triple data[MAXSIZE+1]; /* 非零元三元组表,data[0]未用 */
	int mu,nu,tu; /* 矩阵的行数、列数和非零元个数 */
}TSMatrix;


typedef int Status; //函数返回的状态

#define TRUE		1
#define FALSE		0
#define OK			1
#define ERROR		0
#define INFEASIBLE	-1



 /*三元组稀疏矩阵的基本操作(9个) */
 Status CreateSMatrix(TSMatrix *M)
 { /* 创建稀疏矩阵M */
   int i,m,n;
   ElemType e;
   Status k;
   printf("请输入矩阵的行数,列数,非零元个数：");
   scanf("%d,%d,%d",&(*M).mu,&(*M).nu,&(*M).tu);
   (*M).data[0].i=0; /* 为以下比较顺序做准备 */
   for(i=1;i<=(*M).tu;i++)
   {
     do
     {
       printf("请按行序顺序输入第%d个行非零元素所在的行(1～%d),列(1～%d),元素值：",i,(*M).mu,(*M).nu);
       scanf("%d,%d,%d",&m,&n,&e);
       k=0;
       if(m<1||m>(*M).mu||n<1||n>(*M).nu) /* 行或列超出范围 */
         k=1;
       if(m<(*M).data[i-1].i||m==(*M).data[i-1].i&&n<=(*M).data[i-1].j)/* 行或列的顺序有错 */   
		/*
			上述if条件中，m==(*M).data[i-1].i&&n<=(*M).data[i-1].j 的作用是避免输入的行列和上一次输入的行列值相同
			m<(*M).data[i-1].i 的作用是 输入的的顺序应该是按行序输入，新输入的行号不能小于上一次输入的行号
		*/
         k=1;
     }while(k);
     (*M).data[i].i=m;
     (*M).data[i].j=n;
     (*M).data[i].e=e;
   }
   return OK;
 }

 void DestroySMatrix(TSMatrix *M)
 { /* 销毁稀疏矩阵M */
   (*M).mu=0;
   (*M).nu=0;
   (*M).tu=0;
 }

 void PrintSMatrix(TSMatrix M)
 { /* 输出稀疏矩阵M */
   int i;
   printf("%d行%d列%d个非零元素。\n",M.mu,M.nu,M.tu);
   printf("行  列  元素值\n");
   for(i=1;i<=M.tu;i++)
     printf("%2d%4d%8d\n",M.data[i].i,M.data[i].j,M.data[i].e);
 }

 Status CopySMatrix(TSMatrix M,TSMatrix *T)
 { /* 由稀疏矩阵M复制得到T */
   (*T)=M;
   return OK;
 }

 int comp(int c1,int c2) /* 另加 */
 { /* AddSMatrix函数要用到 */
   int i;
   if(c1<c2)
     i=1;
   else if(c1==c2)
     i=0;
   else
     i=-1;
   return i;
 }

 Status AddSMatrix(TSMatrix M,TSMatrix N,TSMatrix *Q)
 { /* 求稀疏矩阵的和Q=M+N */
   Triple *Mp,*Me,*Np,*Ne,*Qh,*Qe;
   /*只有两个矩阵的行数和列数相等才能进行相加操作*/
   if(M.mu!=N.mu)
     return ERROR;
   if(M.nu!=N.nu)
     return ERROR;
   (*Q).mu=M.mu;
   (*Q).nu=M.nu;
   Mp=&M.data[1]; /* Mp的初值指向矩阵M的非零元素首地址 */
   Np=&N.data[1]; /* Np的初值指向矩阵N的非零元素首地址 */
   Me=&M.data[M.tu]; /* Me指向矩阵M的非零元素尾地址 */
   Ne=&N.data[N.tu]; /* Ne指向矩阵N的非零元素尾地址 */
   Qh=Qe=(*Q).data; /* Qh、Qe的初值指向矩阵Q的非零元素首地址的前一地址 */
   while(Mp<=Me&&Np<=Ne)
   {
     Qe++;
     switch(comp(Mp->i,Np->i))
     {
       case  1: *Qe=*Mp;
                Mp++;
                break;
       case  0: switch(comp(Mp->j,Np->j)) /* M、N矩阵当前非零元素的行相等,继续比较列 */
                {
                  case  1: *Qe=*Mp;
                           Mp++;
                           break;
                  case  0: *Qe=*Mp;
                           Qe->e+=Np->e;
                           if(!Qe->e) /* 元素值为0，不存入压缩矩阵 */
                             Qe--;
                           Mp++;
                           Np++;
                           break;
                  case -1: *Qe=*Np;
                           Np++;
                }
                break;
       case -1: *Qe=*Np;
                Np++;
     }
   }
   if(Mp>Me) /* 矩阵M的元素全部处理完毕 */
     while(Np<=Ne)
     {
       Qe++;
       *Qe=*Np;
       Np++;
     }
   if(Np>Ne) /* 矩阵N的元素全部处理完毕 */
     while(Mp<=Me)
     {
       Qe++;
       *Qe=*Mp;
       Mp++;
     }
   (*Q).tu=Qe-Qh; /* 矩阵Q的非零元素个数 */
   return OK;
 }

 Status SubtSMatrix(TSMatrix M,TSMatrix N,TSMatrix *Q)
 { /* 求稀疏矩阵的差Q=M-N */
   int i;
   for(i=1;i<=N.tu;i++)
     N.data[i].e*=-1; /*矩阵N的非零元素全部添加一个负号，然后再进行两个矩阵相加操作*/
   AddSMatrix(M,N,Q);
   return OK;
 }

 Status MultSMatrix(TSMatrix M,TSMatrix N,TSMatrix *Q)
 { /* 求稀疏矩阵的乘积Q=M*N */
   int i,j,h=M.mu,l=N.nu,Qn=0; //乘积矩阵的行数等于左边矩阵的行数,乘积矩阵的列数等于右边矩阵的列数 
   /* h,l分别为矩阵Q的行、列值,Qn为矩阵Q的非零元素个数，初值为0 */
   ElemType *Qe;
   /*
	矩阵只有当左边矩阵的列数等于右边矩阵的行数时,它们才可以相乘
   */
   if(M.nu!=N.mu)
     return ERROR;
   (*Q).mu=M.mu;
   (*Q).nu=N.nu;
   Qe=(ElemType *)malloc(h*l*sizeof(ElemType)); /* Qe为矩阵Q的临时数组 */
   /* 矩阵Q的第i行j列的元素值存于*(Qe+(i-1)*l+j-1)中，初值为0 */
   for(i=0;i<h*l;i++)
     *(Qe+i)=0; /* 赋初值0 */
   for(i=1;i<=M.tu;i++) /* 矩阵元素相乘，结果累加到Qe */
     for(j=1;j<=N.tu;j++)
       if(M.data[i].j==N.data[j].i)
         *(Qe+(M.data[i].i-1)*l+N.data[j].j-1)+=M.data[i].e*N.data[j].e;
   for(i=1;i<=M.mu;i++)
     for(j=1;j<=N.nu;j++)
       if(*(Qe+(i-1)*l+j-1)!=0)
       {
         Qn++;
         (*Q).data[Qn].e=*(Qe+(i-1)*l+j-1);
         (*Q).data[Qn].i=i;
         (*Q).data[Qn].j=j;
       }
   free(Qe);
   (*Q).tu=Qn;
   return OK;
 }

 Status TransposeSMatrix(TSMatrix M,TSMatrix *T)
 { /* 求稀疏矩阵M的转置矩阵T。 */
   int p,q,col;
   (*T).mu=M.nu;
   (*T).nu=M.mu;
   (*T).tu=M.tu;
   if((*T).tu)
   {
     q=1;
     for(col=1;col<=M.nu;++col)
       for(p=1;p<=M.tu;++p)
         if(M.data[p].j==col)
         {
           (*T).data[q].i=M.data[p].j;
           (*T).data[q].j=M.data[p].i;
           (*T).data[q].e=M.data[p].e;
           ++q;
         }
   }
   return OK;
 }


 Status FastTransposeSMatrix(TSMatrix M,TSMatrix *T)
 { /* 快速求稀疏矩阵M的转置矩阵T。 */
	 int p,q,t,col,*num,*cpot;
	 num=(int *)malloc((M.nu+1)*sizeof(int)); /* 生成数组（[0]不用） */
	 cpot=(int *)malloc((M.nu+1)*sizeof(int)); /* 生成数组（[0]不用） */
	 (*T).mu=M.nu;
	 (*T).nu=M.mu;
	 (*T).tu=M.tu;
	 if((*T).tu)
	 {
		 for(col=1;col<=M.nu;++col)
			 num[col]=0; /* 设初值 */
		 for(t=1;t<=M.tu;++t) /* 求M中每一列含非零元素个数 */
			 ++num[M.data[t].j];
		 cpot[1]=1;
		 for(col=2;col<=M.nu;++col) /* 求第col列中第一个非零元在(*T).data中的序号 */
			 cpot[col]=cpot[col-1]+num[col-1];
		 for(p=1;p<=M.tu;++p)
		 {
			 col=M.data[p].j;
			 q=cpot[col];
			 (*T).data[q].i=M.data[p].j;
			 (*T).data[q].j=M.data[p].i;
			 (*T).data[q].e=M.data[p].e;
			 ++cpot[col];
		 }
	 }
	 free(num);
	 free(cpot);
	 return OK;
 }


/*Test Code*/
 void main()
 {
// 	 TSMatrix A,B,C;
// 	 printf("创建矩阵A: ");
// 	 CreateSMatrix(&A);
// 	 PrintSMatrix(A);
// 	 printf("由矩阵A复制矩阵B: ");
// 	 CopySMatrix(A,&B);
// 	 PrintSMatrix(B);
// 	 DestroySMatrix(&B);
// 	 printf("销毁矩阵B后:\n");
// 	 PrintSMatrix(B);
// 	 printf("创建矩阵B2:(与矩阵A的行、列数相同，行、列分别为%d,%d)\n",A.mu,A.nu);
// 	 CreateSMatrix(&B);
// 	 PrintSMatrix(B);
// 	 printf("矩阵C1(A+B): ");
// 	 AddSMatrix(A,B,&C);
// 	 PrintSMatrix(C);
// 	 DestroySMatrix(&C);
// 	 printf("矩阵C2(A-B): ");
// 	 SubtSMatrix(A,B,&C);
// 	 PrintSMatrix(C);
// 	 DestroySMatrix(&C);
// 	 printf("矩阵C3(A的转置): ");
// 	 TransposeSMatrix(A,&C);
// 	 PrintSMatrix(C);
// 	 DestroySMatrix(&A);
// 	 DestroySMatrix(&B);
// 	 DestroySMatrix(&C);
// 	 printf("创建矩阵A2: ");
// 	 CreateSMatrix(&A);
// 	 PrintSMatrix(A);
// 	 printf("创建矩阵B3:(行数应与矩阵A2的列数相同=%d)\n",A.nu);
// 	 CreateSMatrix(&B);
// 	 PrintSMatrix(B);
// 	 printf("矩阵C5(A*B): ");
// 	 MultSMatrix(A,B,&C);
// 	 PrintSMatrix(C);
// 	 DestroySMatrix(&A);
// 	 DestroySMatrix(&B);
// 	 DestroySMatrix(&C);



	 TSMatrix A,B;
	 printf("创建矩阵A: ");
	 CreateSMatrix(&A);
	 PrintSMatrix(A);
	 FastTransposeSMatrix(A,&B);
	 printf("矩阵B(A的快速转置): ");
	 PrintSMatrix(B);
	 DestroySMatrix(&A);
	 DestroySMatrix(&B);



 }


 
 /*
 
   快速转置算法的测试：

  创建矩阵A: 请输入矩阵的行数,列数,非零元个数：3,4,4
  请按行序顺序输入第1个行非零元素所在的行(1～3),列(1～4),元素值：1,1,2
  请按行序顺序输入第2个行非零元素所在的行(1～3),列(1～4),元素值：1,4,5
  请按行序顺序输入第3个行非零元素所在的行(1～3),列(1～4),元素值：2,1,4
  请按行序顺序输入第4个行非零元素所在的行(1～3),列(1～4),元素值：3,4,5
  3行4列4个非零元素。
  行  列  元素值
  1   1       2
  1   4       5
  2   1       4
  3   4       5
  矩阵B(A的快速转置): 4行3列4个非零元素。
  行  列  元素值
  1   1       2
  1   2       4
  4   1       5
  4   3       5
Press any key to continue
 
 */
