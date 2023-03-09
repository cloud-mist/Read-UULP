/* cp1.c
 *
 * ver1 of cp. 使用read,write，
 *
 * usage: ./cp1 src dest
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644
void oops(char *s1, char *s2);

int main(int argc, char *argv[]) {
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];

    // 检查参数个数
    if (argc != 3) {
        fprintf(stderr, "usage: %s srcfile destfile\n", *argv);
        exit(EXIT_FAILURE);
    }

    // 打开文件
    if ((in_fd = open(argv[1], O_RDONLY)) == -1) {
        oops("Cannot open", argv[1]);
    }
    if ((out_fd = creat(argv[2], COPYMODE)) == -1) {
        oops("Cannot creat", argv[2]);
    }

    // 复制文件
    // 0 for EOF, -1 for err
    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0) {
        if (write(out_fd, buf, n_chars) != n_chars) {
            oops("write err to ", argv[2]);
        }
    }

    // -1 err
    if (n_chars == -1) {
        oops("read file from", argv[1]);
    }

    // 关闭文件
    if (close(in_fd) == -1 || close(out_fd) == -1) {
        oops("Error closing files", "");
    }

    return EXIT_SUCCESS;
}

void oops(char *s1, char *s2) {
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(EXIT_FAILURE);
}
