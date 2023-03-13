/* who3.c
 *
 * who2 存在性能问题
 * */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

#include "./utmplib.c"

#define SHOWHOST
void show_info(struct utmp *utbufp);
void showtime(long timeval);

int main() {
    struct utmp *utbufp,  // pointer to next
        *utmp_next();     // get next pointer

    // open
    if (utmp_open(UTMP_FILE) == -1) {
        perror(UTMP_FILE);
        exit(EXIT_FAILURE);
    }

    // get next record & show
    while ((utbufp = utmp_next()) != ((struct utmp *)NULL)) {
        show_info(utbufp);
    }

    // close
    utmp_close();

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
