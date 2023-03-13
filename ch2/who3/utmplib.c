/* utmplib.c func to buffer reads from utmpfile
 *
 * Funcs
 *      utmp_open(filename)  -> open file
 *      utmp_next()          -> return pointer to next struct
 *      utmp_close()         -> close file
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <utmp.h>

#define NRECS 16
#define UTSIZE (sizeof(struct utmp))
#define NULLUT ((struct utmp *)NULL)

static char utmpbuf[UTSIZE * NRECS];  // storage
static int num_recs;                  // 记录缓冲区记录数量
static int cur_rec;                   // 已经用了记录个数
static int fd_utmp = -1;              // 从哪里读

int utmp_reload();

int utmp_open(char *filename) {
    fd_utmp = open(filename, O_RDONLY);
    cur_rec = num_recs = 0;
    return fd_utmp;
}

struct utmp *utmp_next() {
    struct utmp *recp;
    if (fd_utmp == -1) {  // 打开失败
        return NULLUT;
    }
    if (cur_rec == num_recs && utmp_reload() == 0) {  // 如果没了
        return NULLUT;
    }
    recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}

// Read next bunch of records into buff
int utmp_reload() {
    int amt_read;
    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);  // how many get？
    num_recs = amt_read / UTSIZE;                       // 多少个记录
    cur_rec = 0;                                        // 重置
    return num_recs;
}

// close utmpfile
void utmp_close() {
    if (fd_utmp != -1) {  // 每打开文件，就不需要关
        close(fd_utmp);
    }
}
