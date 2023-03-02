// version0.1 for more
// 读24行， 然后等待特殊命令

/*
 * 问题：
 * more?也会往上滚
 * 当按空格或者q,必须按回车，否则什么也不做
 */
#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();

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
    int see_more(), reply;

    // fgets 从fp指向文件读取下一个输入行，
    // 保存到字符数组line中，最多读取LINELEN-1个字符，
    while (fgets(line, LINELEN, fp)) {
        if (num_of_lines == PAGELEN) {
            reply = see_more();  // 得到返回值
            // 0 是说退出
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

/*
 * 打印信息，等待，
 * q ： no, 空格是yes, 回车是一行
 * */
int see_more() {
    // <<<
    int c;
    printf("\033[7m more?]\033[m");  //  TODO: 不太明白什么意思

    // 读取用户输入命令
    while ((c = getchar()) != EOF) {
        if (c == 'q') {  // q, 退出
            return 0;
        }
        if (c == ' ') {  // 空格， 下一页
            return PAGELEN;
        }
        if (c == '\n') {  // 回车， 1行
            return 1;
        }
    }
    return 0;
    // >>>
}
