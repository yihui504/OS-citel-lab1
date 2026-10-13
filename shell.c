#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 命令最大长度 */

int main(void)
{
    char input[MAX_LINE];               /* 存用户敲的原始命令 */
    char *args[MAX_LINE / 2 + 1];       /* 切好词之后的命令，末尾必须是 NULL */
    int should_run = 1;

    while (should_run) {
        printf("osh>");
        fflush(stdout);

        /* 读一行输入；读不到（Ctrl+D 或出错）就退出 */
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        /* fgets 会把回车也读进来，这里把末尾的 \n 换成字符串结束符 \0 */
        input[strcspn(input, "\n")] = '\0';

        /* 把命令按空格切成一个个词，存进 args[] */
        int argc = 0;
        char *token = strtok(input, " \t");
        while (token != NULL) {
            args[argc++] = token;
            token = strtok(NULL, " \t");
        }
        args[argc] = NULL;

        /* 只敲了回车，什么也不做，重新提示 */
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
                /* 前台：站在原地等孩子干完活 */
                wait(NULL);
            }
        }
    }
    return 0;
}
