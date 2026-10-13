#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 命令最大长度 */
#define HISTORY_NUM 10 /* 最多记住10条命令 */

/* 历史记录：10个格子，每个格子放一条命令字符串 */
char history[HISTORY_NUM][MAX_LINE];
int history_count = 0; /* 开天辟地以来一共记了几条 */

int main(void)
{
    char input[MAX_LINE];               /* 存用户敲的原始命令 */
    char *args[MAX_LINE / 2 + 1];       /* 切好词之后的命令，末尾必须是 NULL */
    int should_run = 1;

    while (should_run) {
        /* 顺手打扫：把已经跑完的后台子进程都回收掉，不留僵尸 */
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ;

        printf("osh>");
        fflush(stdout);

        /* 读一行输入；读不到（Ctrl+D 或出错）就退出 */
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        /* fgets 会把回车也读进来，这里把末尾的 \n 换成字符串结束符 \0 */
        input[strcspn(input, "\n")] = '\0';

        /* 只敲了回车，不记历史也不执行 */
        if (input[0] == '\0')
            continue;

        /* !! 是重跑上一条；先换成真命令再回显，后面照常记历史、执行 */
        if (strcmp(input, "!!") == 0) {
            if (history_count == 0) {
                printf("No commands in history.\n");
                continue;
            }
            strcpy(input, history[(history_count - 1) % HISTORY_NUM]);
            printf("%s\n", input);
        } else if (input[0] == '!' && isdigit((unsigned char)input[1])) {
            /* !N 是重跑编号N的那条 */
            int n = atoi(input + 1);
            if (n >= 1 && n <= history_count) {
                strcpy(input, history[(n - 1) % HISTORY_NUM]);
                printf("%s\n", input);
            } else {
                printf("No such command in history.\n");
                continue;
            }
        }

        /* history 是内置命令：从新到旧列出历史，它自己不占编号 */
        if (strcmp(input, "history") == 0) {
            int n = history_count < HISTORY_NUM ? history_count : HISTORY_NUM;
            for (int i = n; i >= 1; i--)
                printf("%d %s\n", i, history[i - 1]);
            continue;
        }

        /* 记入历史：第 count 条命令放进 count%10 号格子，转一圈覆盖最旧的 */
        strcpy(history[history_count % HISTORY_NUM], input);
        history_count++;

        /* 把命令按空格切成一个个词，存进 args[] */
        int argc = 0;
        char *token = strtok(input, " \t");
        while (token != NULL) {
            args[argc++] = token;
            token = strtok(NULL, " \t");
        }
        args[argc] = NULL;

        /* 整行全是空格的情况，同理跳过 */
        if (argc == 0)
            continue;

        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        /* 最后一个词是 & 就算后台命令：把 & 摘掉，父进程待会不等待 */
        int background = 0;
        if (strcmp(args[argc - 1], "&") == 0) {
            background = 1;
            args[--argc] = NULL;
            if (argc == 0)              /* 整条命令只有一个 &，没东西可跑 */
                continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("osh: fork");
            continue;
        }

        if (pid == 0) {
            /* 子进程：变身成用户要执行的程序 */
            execvp(args[0], args);

            /* execvp 成功就永远回不来了；能走到这里说明命令不存在 */
            fprintf(stderr, "osh: ");
            perror(args[0]);
            exit(1);
        } else {
            if (background) {
                /* 父进程不等，报一下孩子的 pid 就继续 */
                printf("[后台] pid=%d\n", pid);
            } else {
                /* 前台：点名等自家这个孩子，别把后台的死孩子错收走 */
                waitpid(pid, NULL, 0);
            }
        }
    }
    return 0;
}
