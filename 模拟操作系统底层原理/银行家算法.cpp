#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 1000
#define MAX_RESOURCES 1000

int n, m;
int Available[MAX_RESOURCES];
int Max[MAX_PROCESSES][MAX_RESOURCES];
int Allocation[MAX_PROCESSES][MAX_RESOURCES];
int Need[MAX_PROCESSES][MAX_RESOURCES];
int Work[MAX_PROCESSES][MAX_RESOURCES];
int Safe[MAX_PROCESSES];
bool Finish[MAX_PROCESSES];

void showdata() {
    int i, j;
    printf("״̬��\n");
    printf("|Available   |Process     |Max         |Allocation  |Need        |Work        |Finish      \n");
    printf("|");
    for (i = 0; i < n; i++) {
        printf("%-4d", Available[i]);
    }
    printf("|            |            |            |            |            |            \n");
    for (i = 0; i < m; i++) {
        printf("|            |P%-10d|", i);
        for (j = 0; j < n; j++) {
            printf("%-4d", Max[i][j]);
        }
        printf("|");
        for (j = 0; j < n; j++) {
            printf("%-4d", Allocation[i][j]);
        }
        printf("|");
        for (j = 0; j < n; j++) {
            printf("%-4d", Need[i][j]);
        }
        printf("|");
        if (Finish[i] == true) {
            for (j = 0; j < n; j++) {
                printf("%-4d", Work[i][j]);
            }
        }
        else {
            printf("            ");
        }
        printf("|");
        if (Finish[i] == false) {
            printf("%-12s\n", "false");
        }
        else {
            printf("%-12s\n", "true");
        }
    }
}

void safe(int num) {
    int i, j, flag = -1;
    for (i = 0; i < m; i++) {
        if (Finish[i] == false) {
            for (j = 0; j < n; j++) {
                if (Available[j] < Need[i][j])
                    break;
            }
            if (j == n) {
                flag = i;
                break;
            }
        }
    }
    if (flag == -1 && num == m) {
        printf("û�н����������󣬲����Է���\n");
        return;
    }
    if (flag == -1) {
        printf("Ԥ����֮��ȫ�Լ��û���ҵ���ȫ���У��Ӷ������Է���\n");
        return;
    }
    if (flag != -1 && num == 1) {
        Finish[flag] = true;
        for (i = 0; i < n; i++) {
            Work[flag][i] = Available[i];
            Available[i] += Allocation[flag][i];
        }
        printf("P%d����Ԥ����ɹ�\n", flag);
        printf("���н��̾�Ԥ����ɹ�,�ҵ���ȫ���У��ɷ��䣬��ȫ��������\n");
        Safe[num - 1] = flag;
        for (i = m - 1; i > 0; i--) {
            printf("%d->", Safe[i]);
        }
        printf("%d\n", Safe[0]);
        printf("��ȫ���б����£�\n");
        showdata();
        printf("�밴���������\n");
        getchar();
        return;
    }
    if (flag != -1) {
        Finish[flag] = true;
        for (i = 0; i < n; i++) {
            Work[flag][i] = Available[i];
            Available[i] += Allocation[flag][i];
        }
        printf("P%d����Ԥ����ɹ�������Ϊ��ǰ״̬ͼ\n", flag);
        showdata();
        Safe[num - 1] = flag;
        printf("�밴���������\n");
        getchar();
        safe(num - 1);
    }
}

int main() {
    int i, j;
    printf("һ���м�����Դ��");
    scanf("%d", &n);
    printf("����ÿ����Դ��ǰ��ʣ������\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &Available[i]);
    }
    printf("һ���м���������Ҫ������Դ��");
    scanf("%d", &m);
    printf("����ÿ�������ܹ�����ĸ�����Դ��\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &Max[i][j]);
        }
    }
    printf("����ÿ�����̵�ǰ��ӵ�еĸ�����Դ��\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &Allocation[i][j]);
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
        Finish[i] = false;
    }
    printf("��ʼ״̬���£�\n");
    showdata();
    getchar();
    printf("�밴���������\n");
    getchar();
    printf("�������������м��㷨����Ԥ����\n");
    safe(m);
    return 0;
}

