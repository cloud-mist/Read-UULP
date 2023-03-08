// who1.c  -> 0.1 ver Who command
// open, read UTMP file, and show results

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST
void show_info(struct utmp *utbufp);

int main() {
    //
    struct utmp current_record;           // read info into here
    int utmpfd;                           // utmp file 文件指示符
    int reclen = sizeof(current_record);  //

    // 如果打开文件失败，报错,并退出
    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }

    // 如果读取没问题，则打印信息
    while ((read(utmpfd, &current_record, reclen)) == reclen) {
        show_info(&current_record);
    }

    close(utmpfd);  // 关闭文件

    return 0;
}

// show_info
void show_info(struct utmp *utbufp) {
    printf("");
    printf("%-8.8s", utbufp->ut_user);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    printf("%d", utbufp->ut_tv.tv_sec);
    printf(" ");

#ifdef SHOWHOST
    printf("%s", utbufp->ut_host);
#endif
    printf("\n");
}
