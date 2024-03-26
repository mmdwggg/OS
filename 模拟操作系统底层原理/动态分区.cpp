#include <iostream>
#include<stdlib.h>
using namespace std;

#define FREE 0
#define  BUSY 1
#define  MAX_length 1024
int lo = 0;//当前逻辑地址大小
typedef struct freeArea//首先定义空间结构
{
	int flag;//该空间使用状况
	int size;//该空间大小
	int ID;//该空间编号
	int address;//该空间初始位置物理地址
	int logic;//该空间初始位置对应逻辑地址
}Elemtype;

typedef struct Free_Node//空间所组成的结构体
{
	Elemtype date;
	struct Free_Node* front;
	struct Free_Node* next;
}Free_Node, * FNodeList;

FNodeList block_first;
FNodeList block_last;
void init()//初始化 
{
	block_first = new Free_Node;
	block_last = new Free_Node;
	block_first->front = NULL;
	block_first->next = block_last;
	block_last->front = block_first;
	block_last->next = NULL;
	block_last->date.address = 0;
	block_last->date.flag = FREE;
	block_last->date.ID = FREE;
	block_last->date.size = MAX_length;
	block_last->date.logic = FREE;
}

int first_fit(int ID, int size)//首次适应算法
{
	FNodeList temp = (FNodeList)malloc(sizeof(Free_Node));
	Free_Node* p = block_first->next;
	temp->date.ID = ID;
	temp->date.size = size;
	temp->date.flag = BUSY;
	while (p)
	{
		if (p->date.flag == FREE && p->date.size == size)//请求大小刚好满足
		{
			p->date.flag = BUSY;
			p->date.ID = ID;
			p->date.logic = lo;
			return 1;
			break;
		}
		if (p->date.flag == FREE && p->date.size > size)//空间更大，可以一分为二
		{
			temp->next = p;
			temp->front = p->front;
			temp->date.address = p->date.address;
			temp->date.logic = lo;
			p->front->next = temp;
			p->front = temp;
			p->date.address = temp->date.address + temp->date.size;
			p->date.size -= size;
			return 1;
			break;
		}
		p = p->next;
	}
	return 0;
}

int best_fit(int ID, int size)//最佳适应算法
{
	int surplus = 1024;//记录可用内存与需求内存的差值
	FNodeList temp = (FNodeList)malloc(sizeof(Free_Node));
	Free_Node* p = block_first->next;
	temp->date.ID = ID;
	temp->date.size = size;
	temp->date.flag = BUSY;
	Free_Node* q = NULL;//记录最佳位置

	while (p)//继续遍历，找到更加合适的位置
	{
		if (p->date.flag == FREE && p->date.size == size)//正好找到大小相同的空间
		{
			p->date.flag = BUSY;
			p->date.ID = ID;
			p->date.logic = lo;
			return 1;
			break;
		}
		if (p->date.flag == FREE && p->date.size > size)//大小过大，开始判断供需差值
		{
			if (surplus > p->date.size - size)
			{
				surplus = p->date.size - size;
				q = p;
			}

		}
		p = p->next;
	}
	if (q == NULL)//如果没有找到位置
	{
		return 0;
	}
	else//找到了最佳位置
	{
		temp->next = q;
		temp->front = q->front;
		temp->date.address = q->date.address;
		temp->date.logic = lo;
		q->front->next = temp;
		q->front = temp;
		q->date.size = surplus;
		q->date.address += size;
		return 1;
	}
}
int worst_fit(int ID, int size)//最坏适应算法
{
	int surplus = 0;//记录可用内存与需求内存的差值
	FNodeList temp = (FNodeList)malloc(sizeof(Free_Node));
	Free_Node* p = block_first->next;
	temp->date.ID = ID;
	temp->date.size = size;
	temp->date.flag = BUSY;
	Free_Node* q = NULL;//记录最坏位置

	while (p)//继续遍历，找到更坏的位置
	{
		if (p->date.flag == FREE && p->date.size > size)//记录供需差值
		{
			if (surplus < p->date.size - size)
			{
				surplus = p->date.size - size;
				q = p;
			}

		}
		p = p->next;
	}
	if (q == NULL)//如果没有找到位置
	{
		return 0;
	}
	else//找到了最坏位置
	{
		temp->next = q;
		temp->front = q->front;
		temp->date.address = q->date.address;
		temp->date.logic = lo;
		q->front->next = temp;
		q->front = temp;
		q->date.size = surplus;
		q->date.address += size;
		return 1;
	}
}
//实现内存分配
int alloc(int tag)
{
	int ID, size1;
	cout << "请输入编号(正整数)：";
	cin >> ID;
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.ID == ID)//找到要回收的ID区域
		{
			x = 1;
			cout << "输入编号重复";
			break;
		}
		p = p->next;
	}
	if (x == 1)
		return 0;
	cout << "请输入所需内存大小：";
	cin >> size1;
	if (ID <= 0 || size1 <= 0)
	{
		cout << "输入错误！请输入正确的逻辑地址和请求大小：" << endl;
		return 0;
	}

	if (tag == 1)//采用首次适应算法
	{
		if (first_fit(ID, size1))
		{
			cout << "分配成功！" << endl;
			cout << "该区域逻辑地址为"+lo << endl;
			lo += size1;
		}
		else cout << "分配失败！" << endl;
		return 1;
	}
	else if (tag == 2)//采用最佳适应算法
	{
		if (best_fit(ID, size1))
		{
			cout << "分配成功！" << endl;
			cout << "该区域逻辑地址为" + lo << endl;
			lo += size1;
		}
		else cout << "分配失败！" << endl;
		return 1;
	}
	else {//采用最坏适应算法
		if (worst_fit(ID, size1))
		{
			cout << "分配成功！" << endl;
			cout << "该区域逻辑地址为" + lo << endl;
			lo += size1;
		}
		else cout << "分配失败！" << endl;
		return 1;
	}
}

int fre(int ID)//内存回收
{
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.ID == ID&& p->date.flag!=FREE)//找到要回收的ID区域
		{
			x = 1;
			p->date.flag = FREE;
			p->date.ID = FREE;
			if (p->front->date.flag == FREE && p->next->date.flag != FREE)//前有空闲区
			{
				p->front->date.size += p->date.size;
				p->front->next = p->next;
				p->next->front = p->front;
				free(p);
			}
			if (p->front->date.flag != FREE && p->next->date.flag == FREE)//后有空闲区
			{
				p->date.size += p->next->date.size;
				if (p->next->next)
				{
					p->next->next->front = p;
					p->next = p->next->next;
				}
				else p->next = p->next->next;
				free(p->next);
			}
			if (p->front->date.flag == FREE && p->next->date.flag == FREE)//前后都是空闲区
			{
				p->front->date.size += p->date.size + p->next->date.size;
				if (p->next->next)
				{
					p->next->next->front = p->front;
					p->front->next = p->next->next;
				}
				else p->front->next = p->next->next;
				free(p);
				free(p->next);
			}
			if (p->front->date.flag != FREE && p->next->date.flag != FREE)//前后无空闲区
			{

			}
			break;
		}
		p = p->next;
	}
	if (x == 1)
		cout << "回收成功！" << endl;
	if (x == 0)
		cout << "回收失败！" << endl;
	return x;
}
int check(int ID)//通过编号查找
{
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.ID == ID && p->date.flag != FREE)//找到该编号区域
		{
			x = 1;
			cout << "查找成功！" << endl;
			cout << "逻辑地址：" << p->date.logic << endl;
			cout << "物理地址：" << p->date.address << endl;
			cout << "内存大小：" << p->date.size << endl;
			cout << "**************************" << endl;
			break;
		}
		p = p->next;
	}
	if (x == 0)
		cout << "查找失败！" << endl;
	return x;
}
int view(int logic)//逻辑地址查看物理地址
{
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.flag!= FREE)
		{
			if (p->date.logic <= logic && p->date.logic + p->date.size > logic)//找到该逻辑地址对应的空间
			{
				x = 1;
				cout << "查找成功！" << endl;
				cout << "该逻辑地址对应的物理地址为" << p->date.address + (logic - p->date.logic) << endl;//计算其物理位置
				break;
			}
		}
		p = p->next;
	}
	if (x == 0)
		cout << "查找失败！" << endl;
	return x;
}
void show()//查看内存使用情况
{
	cout << "*******内存分配情况*******" << endl;
	Free_Node* p = block_first->next;
	while (p)
	{
		cout << "空间编号：";
		if (p->date.ID == FREE)
			cout << "FREE" << endl;
		else cout << p->date.ID << endl;
		cout << "逻辑地址：";
		if (p->date.ID == FREE)
			cout << "FREE" << endl;
		else cout << p->date.logic << endl;
		cout << "物理地址：" << p->date.address << endl;
		cout << "内存大小：" << p->date.size << endl;
		cout << "分区状态：";
		if (p->date.flag == FREE)
			cout << "空闲" << endl;
		else
			cout << "已分配" << endl;
		cout << "**************************" << endl;
		p = p->next;
	}
}
void menu()//菜单
{
	int tag = 0;
	int ID;
	int logic;
	init();
	cout << "\t\t\t\t\t动态分区分配方式的模拟" << endl;
	while (tag != 8)
	{
		cout << "\t\t**************************请选择要进行的操作**************************" << endl;
		cout << "\t\t\t\t\t1:首次适应算法\n\t\t\t\t\t2:最佳适应算法\n\t\t\t\t\t3:最坏适应算法\n\t\t\t\t\t4:内存回收\n\t\t\t\t\t5:显示内存状况\n\t\t\t\t\t6:查看物理地址\n\t\t\t\t\t7:通过编号查找物理地址\n\t\t\t\t\t8:退出" << endl;
		cin >> tag;
		switch (tag)
		{
		case 1:
			alloc(tag);
			break;
		case 2:
			alloc(tag);
			break;
		case 3:
			alloc(tag);
			break;
		case 4:
			cout << "请输入需要回收的空间编号：";
			cin >> ID;
			fre(ID);
			break;
		case 5:
			show();
			break;
		case 6:
			cout << "请输入需要查看的逻辑地址：";
			cin >> logic;
			view(logic);
			break;
		case 7:
			cout << "请输入查找的空间编号：";
			cin >> ID;
			check(ID);
			break;
		default:
			cout << "输入错误，请重新输入" << endl;

		}
	}

}
int main()
{
	menu();
	return 0;
}
