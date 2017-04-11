#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*  
	串的堆分配存储
	2017年4月3日 15:43:33
*/
typedef struct
{
	char *ch; /* 若是非空串,则按串长分配存储区,否则ch为NULL */
	int length; /* 串长度 */
}HString;

typedef int Status; //用于返回函数的状态

#define TRUE		1
#define FALSE		0
#define OK			1
#define ERROR		0
#define INFEASIBLE	-1
#define OVERFLOW	-2

Status StrAssign(HString *T,char *chars)
 { /* 生成一个其值等于串常量chars的串T */
   int i,j;
   if((*T).ch)
     free((*T).ch); /* 释放T原有空间 */
   i=strlen(chars); /* 求chars的长度i */
   if(!i)
   { /* chars的长度为0 */
     (*T).ch=NULL;
     (*T).length=0;
   }
   else
   { /* chars的长度不为0 */
     (*T).ch=(char*)malloc(i*sizeof(char)); /* 分配串空间 */
     if(!(*T).ch) /* 分配串空间失败 */
       exit(OVERFLOW);
     for(j=0;j<i;j++) /* 拷贝串 */
       (*T).ch[j]=chars[j];
     (*T).length=i;
   }
   return OK;
 }

 Status StrCopy(HString *T,HString S)
 { /* 初始条件:串S存在。操作结果: 由串S复制得串T */
   int i;
   if((*T).ch)
     free((*T).ch); /* 释放T原有空间 */
   (*T).ch=(char*)malloc(S.length*sizeof(char)); /* 分配串空间 */
   if(!(*T).ch) /* 分配串空间失败 */
     exit(OVERFLOW);
   for(i=0;i<S.length;i++) /* 拷贝串 */
     (*T).ch[i]=S.ch[i];
   (*T).length=S.length;
   return OK;
 }

 Status StrEmpty(HString S)
 { /* 初始条件: 串S存在。操作结果: 若S为空串,则返回TRUE,否则返回FALSE */
   if(S.length==0&&S.ch==NULL)
     return TRUE;
   else
     return FALSE;
 }

 int StrCompare(HString S,HString T)
 { /* 若S>T,则返回值>0;若S=T,则返回值=0;若S<T,则返回值<0 */
   int i;
   for(i=0;i<S.length&&i<T.length;++i)
     if(S.ch[i]!=T.ch[i])
       return S.ch[i]-T.ch[i];
   return S.length-T.length;
 }

 int StrLength(HString S)
 { /* 返回S的元素个数,称为串的长度 */
   return S.length;
 }

 Status ClearString(HString *S)
 { /* 将S清为空串 */
   if((*S).ch)
   {
     free((*S).ch);
     (*S).ch=NULL;
   }
   (*S).length=0;
   return OK;
 }

 Status Concat(HString *T,HString S1,HString S2)
 { /* 用T返回由S1和S2联接而成的新串 */
   int i;
   if((*T).ch)
     free((*T).ch); /* 释放旧空间 */
   (*T).length=S1.length+S2.length;
   (*T).ch=(char *)malloc((*T).length*sizeof(char));
   if(!(*T).ch)
     exit(OVERFLOW);
   for(i=0;i<S1.length;i++)
     (*T).ch[i]=S1.ch[i];
   for(i=0;i<S2.length;i++)
     (*T).ch[S1.length+i]=S2.ch[i];
   return OK;
 }

 Status SubString(HString *Sub, HString S,int pos,int len)
 { /* 用Sub返回串S的第pos个字符起长度为len的子串。 */
   /* 其中,1≤pos≤StrLength(S)且0≤len≤StrLength(S)-pos+1 */
   int i;
   if(pos<1||pos>S.length||len<0||len>S.length-pos+1)
     return ERROR;
   if((*Sub).ch)
     free((*Sub).ch); /* 释放旧空间 */
   if(!len) /* 空子串 */
   {
     (*Sub).ch=NULL;
     (*Sub).length=0;
   }
   else
   { /* 完整子串 */
     (*Sub).ch=(char*)malloc(len*sizeof(char));
     if(!(*Sub).ch)
       exit(OVERFLOW);
     for(i=0;i<=len-1;i++)
       (*Sub).ch[i]=S.ch[pos-1+i];
     (*Sub).length=len;
   }
   return OK;
 }

 void InitString(HString *T)
 { /* 初始化(产生空串)字符串T。另加 */
   (*T).length=0;
   (*T).ch=NULL;
 }

 int Index(HString S,HString T,int pos) /* 算法4.1 */
 { /* T为非空串。若主串S中第pos个字符之后存在与T相等的子串, */
   /* 则返回第一个这样的子串在S中的位置,否则返回0 */
   int n,m,i;
   HString sub;
   InitString(&sub);
   if(pos>0)
   {
     n=StrLength(S);
     m=StrLength(T);
     i=pos;
     while(i<=n-m+1)
     {
       SubString(&sub,S,i,m);
       if(StrCompare(sub,T)!=0)
		 ++i;
       else
		return i;
     }
   }
   return 0;
  }

 Status StrInsert(HString *S,int pos,HString T) /* 算法4.4 */
 { /* 1≤pos≤StrLength(S)+1。在串S的第pos个字符之前插入串T */
	 int i;
   if(pos<1||pos>(*S).length+1) /* pos不合法 */
     return ERROR;
   if(T.length) /* T非空,则重新分配空间,插入T */
   {
     (*S).ch=(char*)realloc((*S).ch,((*S).length+T.length)*sizeof(char));
     if(!(*S).ch)
       exit(OVERFLOW);
     for(i=(*S).length-1;i>=pos-1;--i) /* 为插入T而腾出位置 */
       (*S).ch[i+T.length]=(*S).ch[i];
     for(i=0;i<T.length;i++)
       (*S).ch[pos-1+i]=T.ch[i]; /* 插入T */
     (*S).length+=T.length;
   }
   return OK;
 }

 Status StrDelete(HString *S,int pos,int len)
 { /* 从串S中删除第pos个字符起长度为len的子串 */
   int i;
   if((*S).length<pos+len-1)
     exit(ERROR);
   for(i=pos-1;i<=(*S).length-len;i++)
     (*S).ch[i]=(*S).ch[i+len];
   (*S).length-=len;
   (*S).ch=(char*)realloc((*S).ch,(*S).length*sizeof(char));
   return OK;
 }

 Status Replace(HString *S,HString T,HString V)
 { /* 初始条件: 串S,T和V存在,T是非空串（此函数与串的存储结构无关） */
   /* 操作结果: 用V替换主串S中出现的所有与T相等的不重叠的子串 */
   int i=1; /* 从串S的第一个字符起查找串T */
   if(StrEmpty(T)) /* T是空串 */
     return ERROR;
   do
   {
     i=Index(*S,T,i); /* 结果i为从上一个i之后找到的子串T的位置 */
     if(i) /* 串S中存在串T */
     {
       StrDelete(S,i,StrLength(T)); /* 删除该串T */
       StrInsert(S,i,V); /* 在原串T的位置插入串V */
       i+=StrLength(V); /* 在插入的串V后面继续查找串T */
     }
   }while(i);
   return OK;
 }

 void DestroyString()
 { /* 堆分配类型的字符串无法销毁 */
 }

 void StrPrint(HString T)
 { /* 输出T字符串。另加 */
   int i;
   for(i=0;i<T.length;i++)
     printf("%c",T.ch[i]);
   printf("\n");
 }

 void main()
 {
	 int i;
	 char c,*p="God bye!",*q="God luck!";
	 HString t,s,r;
	 InitString(&t); /* HString类型必需初始化 */
	 InitString(&s);
	 InitString(&r);
	 StrAssign(&t,p);
	 printf("串t为: ");
	 StrPrint(t);
	 printf("串长为%d 串空否？%d(1:空 0:否)\n",StrLength(t),StrEmpty(t));
	 StrAssign(&s,q);
	 printf("串s为: ");
	 StrPrint(s);
	 i=StrCompare(s,t);
	 if(i<0)
		 c='<';
	 else if(i==0)
		 c='=';
	 else
		 c='>';
	 printf("串s%c串t\n",c);
	 Concat(&r,t,s);
	 printf("串t联接串s产生的串r为: ");
	 StrPrint(r);
	 StrAssign(&s,"oo");
	 printf("串s为: ");
	 StrPrint(s);
	 StrAssign(&t,"o");
	 printf("串t为: ");
	 StrPrint(t);
	 Replace(&r,t,s);
	 printf("把串r中和串t相同的子串用串s代替后，串r为:\n");
	 StrPrint(r);
	 ClearString(&s);
	 printf("串s清空后，串长为%d 空否？%d(1:空 0:否)\n",StrLength(s),StrEmpty(s));
	 SubString(&s,r,6,4);
	 printf("串s为从串r的第6个字符起的4个字符，长度为%d 串s为: ",s.length);
	 StrPrint(s);
	 StrCopy(&t,r);
	 printf("复制串t为串r,串t为: ");
	 StrPrint(t);
	 StrInsert(&t,6,s);
	 printf("在串t的第6个字符前插入串s后，串t为: ");
	 StrPrint(t);
	 StrDelete(&t,1,5);
	 printf("从串t的第1个字符起删除5个字符后,串t为: ");
	 StrPrint(t);
	 printf("%d是从串t的第1个字符起，和串s相同的第1个子串的位置\n",Index(t,s,1));
	 printf("%d是从串t的第2个字符起，和串s相同的第1个子串的位置\n",Index(t,s,2));
 }

/*
 在VC++6.0中运行结果为：

 串t为: God bye!
 串长为8 串空否？0(1:空 0:否)
 串s为: God luck!
 串s>串t
 串t联接串s产生的串r为: God bye!God luck!
 串s为: oo
 串t为: o
 把串r中和串t相同的子串用串s代替后，串r为:
 Good bye!Good luck!
 串s清空后，串长为0 空否？1(1:空 0:否)
 串s为从串r的第6个字符起的4个字符，长度为4 串s为: bye!
 复制串t为串r,串t为: Good bye!Good luck!
 在串t的第6个字符前插入串s后，串t为: Good bye!bye!Good luck!
 从串t的第1个字符起删除5个字符后,串t为: bye!bye!Good luck!
 1是从串t的第1个字符起，和串s相同的第1个子串的位置
 5是从串t的第2个字符起，和串s相同的第1个子串的位置
Press any key to continue


 */
