/*   显示/etc/passwd 文件的大小 */
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    struct stat infobuf;  // 存储 文件的一些信息

    if (stat("/etc/passwd", &infobuf) == -1) {
        perror("/etc/passwd");
    } else {
        printf("the size of /etc/passwd is:%d\n", infobuf.st_size);
    }

    return 0;
}
