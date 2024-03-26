#include <iostream>
#include<stdlib.h>
using namespace std;

#define FREE 0
#define  BUSY 1
#define  MAX_length 1024
int lo = 0;//��ǰ�߼���ַ��С
typedef struct freeArea//���ȶ���ռ�ṹ
{
	int flag;//�ÿռ�ʹ��״��
	int size;//�ÿռ��С
	int ID;//�ÿռ���
	int address;//�ÿռ��ʼλ�������ַ
	int logic;//�ÿռ��ʼλ�ö�Ӧ�߼���ַ
}Elemtype;

typedef struct Free_Node//�ռ�����ɵĽṹ��
{
	Elemtype date;
	struct Free_Node* front;
	struct Free_Node* next;
}Free_Node, * FNodeList;

FNodeList block_first;
FNodeList block_last;
void init()//��ʼ�� 
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

int first_fit(int ID, int size)//�״���Ӧ�㷨
{
	FNodeList temp = (FNodeList)malloc(sizeof(Free_Node));
	Free_Node* p = block_first->next;
	temp->date.ID = ID;
	temp->date.size = size;
	temp->date.flag = BUSY;
	while (p)
	{
		if (p->date.flag == FREE && p->date.size == size)//�����С�պ�����
		{
			p->date.flag = BUSY;
			p->date.ID = ID;
			p->date.logic = lo;
			return 1;
			break;
		}
		if (p->date.flag == FREE && p->date.size > size)//�ռ���󣬿���һ��Ϊ��
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

int best_fit(int ID, int size)//�����Ӧ�㷨
{
	int surplus = 1024;//��¼�����ڴ��������ڴ�Ĳ�ֵ
	FNodeList temp = (FNodeList)malloc(sizeof(Free_Node));
	Free_Node* p = block_first->next;
	temp->date.ID = ID;
	temp->date.size = size;
	temp->date.flag = BUSY;
	Free_Node* q = NULL;//��¼���λ��

	while (p)//�����������ҵ����Ӻ��ʵ�λ��
	{
		if (p->date.flag == FREE && p->date.size == size)//�����ҵ���С��ͬ�Ŀռ�
		{
			p->date.flag = BUSY;
			p->date.ID = ID;
			p->date.logic = lo;
			return 1;
			break;
		}
		if (p->date.flag == FREE && p->date.size > size)//��С���󣬿�ʼ�жϹ����ֵ
		{
			if (surplus > p->date.size - size)
			{
				surplus = p->date.size - size;
				q = p;
			}

		}
		p = p->next;
	}
	if (q == NULL)//���û���ҵ�λ��
	{
		return 0;
	}
	else//�ҵ������λ��
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
int worst_fit(int ID, int size)//���Ӧ�㷨
{
	int surplus = 0;//��¼�����ڴ��������ڴ�Ĳ�ֵ
	FNodeList temp = (FNodeList)malloc(sizeof(Free_Node));
	Free_Node* p = block_first->next;
	temp->date.ID = ID;
	temp->date.size = size;
	temp->date.flag = BUSY;
	Free_Node* q = NULL;//��¼�λ��

	while (p)//�����������ҵ�������λ��
	{
		if (p->date.flag == FREE && p->date.size > size)//��¼�����ֵ
		{
			if (surplus < p->date.size - size)
			{
				surplus = p->date.size - size;
				q = p;
			}

		}
		p = p->next;
	}
	if (q == NULL)//���û���ҵ�λ��
	{
		return 0;
	}
	else//�ҵ����λ��
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
//ʵ���ڴ����
int alloc(int tag)
{
	int ID, size1;
	cout << "��������(������)��";
	cin >> ID;
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.ID == ID)//�ҵ�Ҫ���յ�ID����
		{
			x = 1;
			cout << "�������ظ�";
			break;
		}
		p = p->next;
	}
	if (x == 1)
		return 0;
	cout << "�����������ڴ��С��";
	cin >> size1;
	if (ID <= 0 || size1 <= 0)
	{
		cout << "���������������ȷ���߼���ַ�������С��" << endl;
		return 0;
	}

	if (tag == 1)//�����״���Ӧ�㷨
	{
		if (first_fit(ID, size1))
		{
			cout << "����ɹ���" << endl;
			cout << "�������߼���ַΪ"+lo << endl;
			lo += size1;
		}
		else cout << "����ʧ�ܣ�" << endl;
		return 1;
	}
	else if (tag == 2)//���������Ӧ�㷨
	{
		if (best_fit(ID, size1))
		{
			cout << "����ɹ���" << endl;
			cout << "�������߼���ַΪ" + lo << endl;
			lo += size1;
		}
		else cout << "����ʧ�ܣ�" << endl;
		return 1;
	}
	else {//�������Ӧ�㷨
		if (worst_fit(ID, size1))
		{
			cout << "����ɹ���" << endl;
			cout << "�������߼���ַΪ" + lo << endl;
			lo += size1;
		}
		else cout << "����ʧ�ܣ�" << endl;
		return 1;
	}
}

int fre(int ID)//�ڴ����
{
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.ID == ID&& p->date.flag!=FREE)//�ҵ�Ҫ���յ�ID����
		{
			x = 1;
			p->date.flag = FREE;
			p->date.ID = FREE;
			if (p->front->date.flag == FREE && p->next->date.flag != FREE)//ǰ�п�����
			{
				p->front->date.size += p->date.size;
				p->front->next = p->next;
				p->next->front = p->front;
				free(p);
			}
			if (p->front->date.flag != FREE && p->next->date.flag == FREE)//���п�����
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
			if (p->front->date.flag == FREE && p->next->date.flag == FREE)//ǰ���ǿ�����
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
			if (p->front->date.flag != FREE && p->next->date.flag != FREE)//ǰ���޿�����
			{

			}
			break;
		}
		p = p->next;
	}
	if (x == 1)
		cout << "���ճɹ���" << endl;
	if (x == 0)
		cout << "����ʧ�ܣ�" << endl;
	return x;
}
int check(int ID)//ͨ����Ų���
{
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.ID == ID && p->date.flag != FREE)//�ҵ��ñ������
		{
			x = 1;
			cout << "���ҳɹ���" << endl;
			cout << "�߼���ַ��" << p->date.logic << endl;
			cout << "�����ַ��" << p->date.address << endl;
			cout << "�ڴ��С��" << p->date.size << endl;
			cout << "**************************" << endl;
			break;
		}
		p = p->next;
	}
	if (x == 0)
		cout << "����ʧ�ܣ�" << endl;
	return x;
}
int view(int logic)//�߼���ַ�鿴�����ַ
{
	Free_Node* p = block_first->next;
	int x = 0;
	while (p)
	{
		if (p->date.flag!= FREE)
		{
			if (p->date.logic <= logic && p->date.logic + p->date.size > logic)//�ҵ����߼���ַ��Ӧ�Ŀռ�
			{
				x = 1;
				cout << "���ҳɹ���" << endl;
				cout << "���߼���ַ��Ӧ�������ַΪ" << p->date.address + (logic - p->date.logic) << endl;//����������λ��
				break;
			}
		}
		p = p->next;
	}
	if (x == 0)
		cout << "����ʧ�ܣ�" << endl;
	return x;
}
void show()//�鿴�ڴ�ʹ�����
{
	cout << "*******�ڴ�������*******" << endl;
	Free_Node* p = block_first->next;
	while (p)
	{
		cout << "�ռ��ţ�";
		if (p->date.ID == FREE)
			cout << "FREE" << endl;
		else cout << p->date.ID << endl;
		cout << "�߼���ַ��";
		if (p->date.ID == FREE)
			cout << "FREE" << endl;
		else cout << p->date.logic << endl;
		cout << "�����ַ��" << p->date.address << endl;
		cout << "�ڴ��С��" << p->date.size << endl;
		cout << "����״̬��";
		if (p->date.flag == FREE)
			cout << "����" << endl;
		else
			cout << "�ѷ���" << endl;
		cout << "**************************" << endl;
		p = p->next;
	}
}
void menu()//�˵�
{
	int tag = 0;
	int ID;
	int logic;
	init();
	cout << "\t\t\t\t\t��̬�������䷽ʽ��ģ��" << endl;
	while (tag != 8)
	{
		cout << "\t\t**************************��ѡ��Ҫ���еĲ���**************************" << endl;
		cout << "\t\t\t\t\t1:�״���Ӧ�㷨\n\t\t\t\t\t2:�����Ӧ�㷨\n\t\t\t\t\t3:���Ӧ�㷨\n\t\t\t\t\t4:�ڴ����\n\t\t\t\t\t5:��ʾ�ڴ�״��\n\t\t\t\t\t6:�鿴�����ַ\n\t\t\t\t\t7:ͨ����Ų��������ַ\n\t\t\t\t\t8:�˳�" << endl;
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
			cout << "��������Ҫ���յĿռ��ţ�";
			cin >> ID;
			fre(ID);
			break;
		case 5:
			show();
			break;
		case 6:
			cout << "��������Ҫ�鿴���߼���ַ��";
			cin >> logic;
			view(logic);
			break;
		case 7:
			cout << "��������ҵĿռ��ţ�";
			cin >> ID;
			check(ID);
			break;
		default:
			cout << "�����������������" << endl;

		}
	}

}
int main()
{
	menu();
	return 0;
}
