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
    printf("状态表：\n");
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
        printf("没有进程满足需求，不可以分配\n");
        return;
    }
    if (flag == -1) {
        printf("预分配之后安全性检查没有找到安全序列，从而不可以分配\n");
        return;
    }
    if (flag != -1 && num == 1) {
        Finish[flag] = true;
        for (i = 0; i < n; i++) {
            Work[flag][i] = Available[i];
            Available[i] += Allocation[flag][i];
        }
        printf("P%d进程预分配成功\n", flag);
        printf("所有进程均预分配成功,找到安全序列，可分配，安全序列如下\n");
        Safe[num - 1] = flag;
        for (i = m - 1; i > 0; i--) {
            printf("%d->", Safe[i]);
        }
        printf("%d\n", Safe[0]);
        printf("安全序列表如下：\n");
        showdata();
        printf("请按任意键继续\n");
        getchar();
        return;
    }
    if (flag != -1) {
        Finish[flag] = true;
        for (i = 0; i < n; i++) {
            Work[flag][i] = Available[i];
            Available[i] += Allocation[flag][i];
        }
        printf("P%d进程预分配成功，以下为当前状态图\n", flag);
        showdata();
        Safe[num - 1] = flag;
        printf("请按任意键继续\n");
        getchar();
        safe(num - 1);
    }
}

int main() {
    int i, j;
    printf("一共有几种资源：");
    scanf("%d", &n);
    printf("输入每种资源当前的剩余数量\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &Available[i]);
    }
    printf("一共有几道进程需要分配资源：");
    scanf("%d", &m);
    printf("输入每道进程总共所需的各种资源数\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &Max[i][j]);
        }
    }
    printf("输入每道进程当前所拥有的各种资源数\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &Allocation[i][j]);
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
        Finish[i] = false;
    }
    printf("初始状态如下：\n");
    showdata();
    getchar();
    printf("请按任意键继续\n");
    getchar();
    printf("接下来调用银行家算法进行预分配\n");
    safe(m);
    return 0;
}

