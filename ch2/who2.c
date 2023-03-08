// who2.c  -> 0.2 ver Who command
// open, read UTMP file, and show results
//
/* who1 存在两个问题
 *      1.空白记录
 *      2.时间显示问题，显示的是秒数，而不是平常看到的时间
 *
 * */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST
void show_info(struct utmp *utbufp);
void showtime(long timeval);

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
    // NOTE:修复空白行问题，过滤登陆的用户，当ut_type 的值为7,
    // 表示是已经登陆的用户
    if (utbufp->ut_type != USER_PROCESS) {
        return;
    }
    printf("%-8.8s", utbufp->ut_user);  // name
    printf(" ");                        //
    printf("%-8.8s", utbufp->ut_line);  // tty
    printf(" ");                        //
    showtime(utbufp->ut_tv.tv_sec);     // time
    printf(" ");

#ifdef SHOWHOST
    printf("%s", utbufp->ut_host);  // host
#endif
    printf("\n");
}

// NOTE: 修复时间显示问题
// 使用ctime,转换成string, 然后截取需要的区间
void showtime(long timeval) {
    char *cp;                   // to hold addr of time
    cp = ctime(&timeval);       // convert time to string
    printf("%12.12s", cp + 4);  // pick 12 chars from pos 4
}
