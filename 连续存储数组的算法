
# include <stdio.h>
# include <stdlib.h>
# include <malloc.h>
/************************************************************************/
/* 连续存储数组的算法
	 2017年2月25日 08:48:14                            */
/************************************************************************/

//这里定义了一个数据类型，该数据类型的名字为struct Array
struct Array 
{
	int *pArray;//存储的是数组的第一个元素的地址
	int length;//数组所能容纳元素的最大个数(数组的容量)
	int count;//有效元素的个数

};

//功能函数声明
void init_Array(struct Array * array,int length);//初始化数组
bool append_Array(struct Array *array,int val);//追加元素
bool insert_Array(struct Array *aray,int pos,int val);//插入元素
bool delete_Array(struct Array *aray,int pos,int *val);//删除元素
bool is_Empty(struct Array *array);//判断是否为空
bool is_Full(struct Array *array);//判断元素是否满员
void sort_Array(struct Array *array);//对数组排序
void show_Array(struct Array *array);//显示数组元素
void reverse_Array(struct Array *array);//倒置数组的元素


int main(void)
{
	struct Array array;
	int val;
	init_Array(&array,6);//初始化数组的容量，分配内存
	append_Array(&array,1);
	append_Array(&array,2);
	append_Array(&array,3);
	append_Array(&array,4);
	append_Array(&array,5);
	show_Array(&array);
	insert_Array(&array,2,9);
	show_Array(&array);
	delete_Array(&array,5,&val);
	printf("您删除的元素是：%d\n",val);
	printf("删除后的数组:\n");
	show_Array(&array);
	sort_Array(&array);
	printf("排序后的数组元素：\n");
	show_Array(&array);
	reverse_Array(&array);
	printf("倒置后的数组元素：\n");
	show_Array(&array);
	return 0;
}

//初始化数组
void init_Array( struct Array* array,int length )
{
	array->pArray=(int *)malloc(sizeof(int)*length);
	if(NULL == array->pArray)
	{
		printf("动态内存分配失败！\n");
		exit(-1);
	}
	else
	{
		array->length=length;
		array->count=0;
	}
	return; 
}

//判断是否为空
bool is_Empty(struct Array *array)
{
	if(array->count == 0)
		return true;
	else
		return false;

}

//判断是否满员
bool is_Full(struct Array *array)
{
	if(array->count==array->length)
		return true;
	else
		return false;
}

//显示
void show_Array(struct Array *array)
{
	int i;
	if(is_Empty(array))
	{
		printf("数组为空！\n");
	}
	for(i=0;i<array->count;i++ )
		printf("%d	",array->pArray[i]);
		printf("\n");

}
//追加
bool append_Array(struct Array *array,int val)
{
	if(is_Full(array))
		return false;
	array->pArray[array->count]=val;
	(array->count)++;
	return true;
}
//插入
bool insert_Array(struct Array *array,int pos,int val)
{
	int temp=0;
	if(is_Full(array))
		return false;
	for(int i=array->count-1;i>=pos-1;i--)
		array->pArray[i+1]=array->pArray[i];
	
	array->pArray[pos-1]=val;
	array->count++;
	return true;
}

//删除
bool delete_Array(struct Array *array,int pos,int *val)
{
	if(is_Empty(array))
 		return false;
	*val=array->pArray[pos-1];
 	for(int i=pos; i<array->count;i++)
		array->pArray[i-1]=array->pArray[i];
 
	array->count--;

	return true;
}

//排序
void sort_Array(struct Array *array)
{
	int i,j;
	int temp;
	for(i=0;i<array->count;i++)
		for(j=i+1; j<array->count ;j++)
		{
			if(array->pArray[i]>array->pArray[i+1])
			{
				temp=array->pArray[i];
				array->pArray[i]=array->pArray[i+1];
				array->pArray[i+1]=temp;

			}
		}
}

//倒置
void reverse_Array(struct Array *array)
{
	int i=0;
	int j=array->count-1;
	int temp=0;
	while(i<j)
	{
		temp=array->pArray[i];
		array->pArray[i]=array->pArray[j];
		array->pArray[j]=temp;
		++i;
		--j;
	}

	
}
