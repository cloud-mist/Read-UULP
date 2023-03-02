// version0.2 for more

/*
 * v1问题：
 * more?也会往上滚
 * 当按空格或者q,必须按回车，否则什么也不做
 */

// 解决问题：
// 程序标准输入是ls的输出，将其分页显示到屏幕上，当more需要用户输入时，它可以从/dev/tty获取数据
#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more(FILE *);

int main(int argc, char *argv[]) {
    // <<<
    // 1.从哪里读？
    // 如果从标准输入读，即只有一个参数（命令本身），就调用do_more 从标准输入读
    // 如果从所给文件序列读，就读程序后面带的多个文件名
    FILE *fp;
    if (argc == 1) {
        do_more(stdin);
    } else {
        while (argc--) {
            if ((fp = fopen(*++argv, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else {
                exit(1);
            }
        }
    }
    return 0;
    // >>>
}

/*
 *
 * 读PAGELEN行，然后调用see_more
 *
 * */
void do_more(FILE *fp) {
    // <<<
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(FILE *), reply;

    FILE *fp_tty;
    fp_tty = fopen("/dev/tty", "r");  // NEW: cmd stream
    if (fp_tty == NULL) {
        exit(1);
    }

    while (fgets(line, LINELEN, fp)) {
        if (num_of_lines == PAGELEN) {
            reply = see_more(fp_tty);
            if (reply == 0) {
                break;
            }
            num_of_lines -= reply;
        }

        if (fputs(line, stdout) == EOF) {
            exit(1);
        }
        num_of_lines++;
    }
    // >>>
}

int see_more(FILE *cmd) {
    // <<<
    int c;
    printf("\033[7m more?]\033[m");

    // 读取用户输入命令
    while ((c = getc(cmd)) != EOF) {  // New: 从tty读
        if (c == 'q') {
            return 0;
        }
        if (c == ' ') {
            return PAGELEN;
        }
        if (c == '\n') {
            return 1;
        }
    }
    return 0;
    // >>>
}
