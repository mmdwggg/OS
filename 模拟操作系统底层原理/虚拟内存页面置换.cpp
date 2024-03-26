#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_VISIRNUM 100 
void ShowVisit(int a[], int num)
{
	printf("��������:");
	for (int i = 0; i < num; i++)
	{
		printf("%2d", a[i]);
	}
	printf("\n");
}

void ShowRam(int a[], int num,int re)
{
	printf("�ڴ��:");
	for (int i = 0; i < num; i++)
	{
		printf("%3d", a[i]);
	}
	printf("\n                  ");
	for (int i = 0; i < num; i++)
	{
		if (i == re) {
			printf("  -");
		}
		else {
			printf("   ");
		}
	}
	printf("\n");
}

void ShowFinal(int replaceNum, int lackingNum, int VisitNum,int a[])
{
	int i;
	printf("\nȱҳ������%3d\n", lackingNum);
	printf("�û�������%3d\n", replaceNum);
	printf("ȱҳ�ʣ�%.2f%%\n", lackingNum * 1.0 / VisitNum * 100);
	printf("������: %.2f%%\n", (1 - lackingNum * 1.0 / VisitNum) * 100);
	printf("��̭˳��:");
	for (i = 0; a[i] != -1; i++) {
		printf("%3d", a[i]);
	}
	printf("\n");
}

void Init(int a[], int num)
{
	for (int i = 0; i < num; i++) { a[i] = -1; }
}

int searchfuture(int fork[], int now, int VisitNum, int nowmessage)
{
	int i = now + 1;
	for (i; i < VisitNum; i++)
	{
		if (fork[i] == nowmessage)
		{
			return i;
		}
	}
	return MAX_VISIRNUM;
}

void FIFO(int fork[], int PageNum, int PhyNum, int VisitNum)
{
	int ram[MAX_VISIRNUM]; Init(ram, PhyNum);
	int replace[MAX_VISIRNUM]; Init(replace, VisitNum);
	int point = 0;
	int judge = -1;
	int replaceNum = 0;//�û����� 
	int lackingNum = 0;//ȱҳ���� 
	for (int i = 0; i < VisitNum; i++)
	{
		for (int j = 0; j < PhyNum; j++)
		{
			if (ram[j] == fork[i]) //һ�����ڴ�֮�ڣ�ֱ����� 
			{
				printf("�����ڴ�%d��", fork[i]);
				ShowRam(ram, PhyNum,-1);
				judge = 1;
				break;
			}
			if (ram[j] == -1)//�����ڴ�֮�в���û���� ��Ҳֱ����� 
			{
				ram[j] = fork[i];
				lackingNum++;
				printf("�����ڴ�%d��", fork[i]);
				ShowRam(ram, PhyNum,-1);
				judge = 1;
				break;
			}
		}
		if (judge == -1) {
			replace[replaceNum] = ram[point];
			ram[point] = fork[i];                    //���ڴ�֮�ڲ����Ѿ����ˣ�FIFO�û� 
			printf("�����ڴ�%d��", fork[i]);
			ShowRam(ram, PhyNum,point);
			point = (point + 1) % PhyNum;
			replaceNum++;
			lackingNum++;
		}
		judge = -1;
	}
	ShowFinal(replaceNum, lackingNum, VisitNum,replace);
}

void LRU(int fork[], int PageNum, int PhyNum, int VisitNum)
{
	int ram[MAX_VISIRNUM]; Init(ram, PhyNum);
	int time[MAX_VISIRNUM]; Init(time, PhyNum);//time��¼�����Ѿ��еĿ�Ŷ��ûʹ���� 
	int replace[MAX_VISIRNUM]; Init(replace, VisitNum);
	int judge = -1;
	int replaceNum = 0;//�û����� 
	int lackingNum = 0;//ȱҳ���� 

	for (int i = 0; i < VisitNum; i++)
	{
		for (int k = 0; k < PhyNum; k++)
		{
			if (time[k] != -1) { time[k]++; }
		}
		for (int j = 0; j < PhyNum; j++)
		{
			if (ram[j] == fork[i]) //һ�����ڴ�֮�ڣ�ֱ����� 
			{
				printf("�����ڴ�%d��", fork[i]);
				ShowRam(ram, PhyNum,-1);
				time[j] = 0;
				//printf("time:");  for (int op = 0; op < PhyNum; op++) { printf("%2d", time[op]); }printf("\n");
				judge = 1;
				break;
			}
			if (ram[j] == -1)//�����ڴ�֮�в���û���� ��Ҳֱ����� 
			{
				ram[j] = fork[i];
				lackingNum++;
				printf("�����ڴ�%d��", fork[i]);
				ShowRam(ram, PhyNum,-1);
				time[j] = 0;
				judge = 1;
				//printf("time:"); for (int op = 0; op < PhyNum; op++) { printf("%2d", time[op]); }printf("\n");
				break;
			}
		}
		if (judge == -1) {                     //���ڴ�֮�ڲ����Ѿ����ˣ�LRU�û� 
			int max = 0;
			for (int k = 0; k < PhyNum; k++)
			{
				if (time[k] > time[max])
					max = k;
			}
			replace[replaceNum] = ram[max];
			ram[max] = fork[i];
			time[max] = 0;
			replaceNum++;
			lackingNum++;
			printf("�����ڴ�%d��", fork[i]);
			ShowRam(ram, PhyNum,max);
			//printf("time:"); for (int op = 0; op < PhyNum; op++) { printf("%2d", time[op]); }printf("\n");
		}
		judge = -1;
	}
	ShowFinal(replaceNum, lackingNum, VisitNum,replace);
}

void OPT(int fork[], int PageNum, int PhyNum, int VisitNum)
{
	int ram[MAX_VISIRNUM]; Init(ram, PhyNum);
	int future[MAX_VISIRNUM]; Init(future, PhyNum);//future��¼����δ�����Ҫʹ�õ� 
	int replace[MAX_VISIRNUM]; Init(replace, VisitNum);
	int judge = -1;
	int replaceNum = 0;//�û����� 
	int lackingNum = 0;//ȱҳ���� 

	for (int i = 0; i < VisitNum; i++)
	{

		for (int j = 0; j < PhyNum; j++)
		{
			if (ram[j] == fork[i]) //һ�����ڴ�֮�ڣ�ֱ����� 
			{
				printf("�����ڴ�%d��", fork[i]);
				ShowRam(ram, PhyNum,-1);
				future[j] = searchfuture(fork, i, VisitNum, ram[j]);
				judge = 1;
				break;
			}
			if (ram[j] == -1)//�����ڴ�֮�в���û���� ��Ҳֱ����� 
			{
				ram[j] = fork[i];
				lackingNum++;
				printf("�����ڴ�%d��", fork[i]);
				ShowRam(ram, PhyNum,-1);
				judge = 1;
				future[j] = searchfuture(fork, i, VisitNum, ram[j]);
				break;
			}
		}
		if (judge == -1) {                     //���ڴ�֮�ڲ����Ѿ����ˣ�LRU�û� 
			int max = 0;
			for (int k = 0; k < PhyNum; k++)
			{
				if (future[k] > future[max])
					max = k;
			}
			replace[replaceNum] = ram[max];
			ram[max] = fork[i];
			future[max] = searchfuture(fork, i, VisitNum, ram[max]);
			replaceNum++;
			lackingNum++;
			printf("�����ڴ�%d��", fork[i]);
			ShowRam(ram, PhyNum,max);
		}
		judge = -1;
	}
	ShowFinal(replaceNum, lackingNum, VisitNum,replace);


}

int main()
{
	int PageNum=10;                 //ҳ������ 
	int PhyNum;                 //���������  
	int VisitNum;              //�������еĳ���
	int Visit[MAX_VISIRNUM];
	printf("�������ڴ��������\n");
	scanf("%d", &PhyNum);
	printf("������������г���\n");
	scanf("%d", &VisitNum);

	srand((unsigned)time(NULL));      //����������� 
	for (int i = 0; i < VisitNum; i++)
	{
		Visit[i] = rand() % PageNum;
	}
	ShowVisit(Visit, VisitNum);

	while (1) {
		int fork[MAX_VISIRNUM];          //fork������Ǹ�����Visit���飬�൱�ڳ�ʼ��������� 
		for (int i = 0; i < VisitNum; i++)
		{
			fork[i] = Visit[i];
		}
		printf("��ѡ������ʹ�õ��û��㷨:\n");
		printf("1.FIFO 2.LRU 3.OPT 4.��������������� 5.SIGN OUT\n");
		int want;
		scanf("%d", &want);
		switch (want) {
		case 1:
			ShowVisit(Visit, VisitNum);
			FIFO(fork, PageNum, PhyNum, VisitNum); printf("\n");
			break;
		case 2:
			ShowVisit(Visit, VisitNum);
			LRU(fork, PageNum, PhyNum, VisitNum); printf("\n");
			break;
		case 3:
			ShowVisit(Visit, VisitNum);
			OPT(fork, PageNum, PhyNum, VisitNum); printf("\n");
			break;
		case 4:
			printf("�������ڴ��������\n");
			scanf("%d", &PhyNum);
			break;
		case 5:
			printf("�˳�");
			return 0;
		default:
			printf("Խ��,����������\n");
			break;
		}
	}
	return 0;
}

