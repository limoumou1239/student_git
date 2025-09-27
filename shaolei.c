#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define X 10
#define Y 10
int map[X + 2][Y + 2];
int record[X + 2][Y + 2];
int x, y;
char choice[100];
void START() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < X + 2; i++) {
        for (int j = 0; j < Y + 2; j++) {
            map[i][j] = 0;
            record[i][j] = 0;
        }
    }
}
int ss;
void BURY() {
    int sum;
    printf("请输入雷的数量：");
    scanf("%d", &sum);
    while (sum <= 0 || sum >= 100) {
        printf("输入错误！\n");
        printf("请输入雷的数量：");
        scanf("%d", &sum);
    }
    printf("输入要点击的格子坐标（行 列）:");
    scanf("%d %d", &x, &y);
    int a = 0;
    while (a < sum) {
        int m = rand() % X + 1;
        int n = rand() % Y + 1;
        if (map[m][n] == 0 && (m != x || n != y)) {
            map[m][n] = -1;
            a++;
        }
    }
}

void NUMBER() {
    for (int i = 1; i <= X; i++) {
        for (int j = 1; j <= Y; j++) {
            if (map[i][j] != -1) {
                int mines = 0;
                for (int b = i - 1; b <= i + 1; b++) {
                    for (int c = j - 1; c <= j + 1; c++) {
                        if (map[b][c] == -1 && (b != i || c != j)) {
                            mines++;
                        }
                    }
                }
                map[i][j] = mines;
            }
        }
    }
}

void PRINT() {
    for (int i = 0; i <= X; i++) {
        for (int j = 0; j <= Y; j++) {
            if (i == 0 && j > 0) {
                printf("%2d ", j);
            }
            else if (j == 0 && i > 0) {
                printf("%2d ", i);
            }
            else if (record[i][j]) {
                if (map[i][j] == -1) {
                    printf(" * ");
                }
                else {
                    printf("%2d ", map[i][j]);
                }
            }
            else {
                printf(" - ");
            }
        }
        printf("\n");
    }
}

void RECURSION(int i, int j) {
    if (i < 1 || i > X || j < 1 || j > Y || record[i][j] || map[i][j] != 0) {
        return;
    }
    record[i][j] = 1;
    RECURSION(i + 1, j);
    RECURSION(i - 1, j);
    RECURSION(i, j + 1);
    RECURSION(i, j - 1);
}

int main() {
    char a[100];
    while (1) {
        printf("是否开始游戏（开始/退出）:");
        scanf("%s", a);
        if (strcmp(a, "开始") == 0) {
            START();
            BURY();
            NUMBER();
            if (x < 1 || x > X || y < 1 || y > Y) {
                printf("无效的坐标，请重新输入。\n");
            }
            else {
                record[x][y] = 1;
                printf("周围有 %d 个地雷\n", map[x][y]);
                if (map[x][y] == 0) {
                    RECURSION(x, y);
                }
                PRINT();
                while (1) {
                    printf("输入要点击的格子坐标（行 列）:");
                    scanf("%d %d", &x, &y);
                    if (x < 1 || x > X || y < 1 || y > Y) {
                        printf("无效的坐标，请重新输入。\n");
                    }
                    else if (record[x][y]) {
                        printf("这个格子已经被点过了！\n");
                    }
                    else {
                        record[x][y] = 1;
                        if (map[x][y] == -1) {
                            printf("Boom!恭喜你踩到地雷了！\n游戏结束。\n");
                            PRINT();
                            break;
                        }
                        else {
                            printf("周围有 %d 个地雷\n", map[x][y]);
                            if (map[x][y] == 0) {
                                RECURSION(x, y);
                            }
                            PRINT();
                        }
                    }
                    int allCleared = 1;
                    for (int i = 1; i <= X; i++) {
                        for (int j = 1; j <= Y; j++) {
                            if (map[i][j] != -1 && !record[i][j]) {
                                allCleared = 0;
                                break;
                            }
                        }
                    }
                    if (allCleared) {
                        printf("恭喜，你赢了！\n");
                        break;
                    }
                }
                printf("要再来一次吗？（再来一次/退出游戏）：");
                scanf("%s", choice);
                if (strcmp(choice, "退出游戏") == 0) {
                    printf("再见\n");
                    break;
                }
                else if (strcmp(choice, "再来一次") == 0) {
                    continue;
                }
            }
        }
        else if (strcmp(a, "退出") == 0) {
            printf("再见\n");
            break;
        }
        else {
            printf("输入错误！\n");
        }
    }
    return 0;
}