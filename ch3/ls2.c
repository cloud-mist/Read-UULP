/* ls2.c
 *
 * ls1输出的格式存在很多问题
 * mode：        需要转换成_rw_r__r__ 的格式 -> 通过掩码与来实现
 * groupid和uid: 需要转换成名字              -> 通过getpwuid
 * 时间：        需要转换成可读的形式        -> ctime
 *
 */

#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void do_ls(char dirname[]);
void mode_to_letters(int mode, char str[]);
char *uid_to_name(uid_t uid);
void show_info(char *fname, struct stat *buf);
void dostat(char *fname);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_ls(".");
    } else {
        while (--argc) {
            printf("%s:\n", *++argv);
            do_ls(*argv);
        }
    }

    return 0;
}

void do_ls(char dirname[]) {
    // <<<
    DIR *dir_ptr;
    struct dirent *direntp;  // 每个实体

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls2: cannot open %s\n", dirname);
    } else {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            dostat(direntp->d_name);
        }
        closedir(dir_ptr);
    }
    // >>>
}

void dostat(char *fname) {
    struct stat info;
    if ((stat(fname, &info)) == -1) {
        perror(fname);
    } else {
        show_info(fname, &info);
    }
}

// 将模式转换成和ls -l 一致的形式
void mode_to_letters(int mode, char str[]) {
    // <<<
    strcpy(str, "----------");  // 初始化

    if (S_ISDIR(mode)) str[0] = 'd';  // 目录
    if (S_ISCHR(mode)) str[0] = 'c';  // char devices
    if (S_ISBLK(mode)) str[0] = 'b';  // block devices

    // user [1-3]
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    // group [4-6]
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    // other [7-9]
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
    // >>>
}

// 通过uid查找对应的用户名, 借用pwd.h, getpwuid函数
char *uid_to_name(uid_t uid) {
    // <<<
    struct passwd *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pw_ptr->pw_name;
    }
    // >>>
}

// 通过gid查找对应组名，借用grp.h, getgrgid()
char *gid_to_name(gid_t gid) {
    // <<<
    struct group *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return grp_ptr->gr_name;
    }
    // >>>
}

// 显示文件信息
void show_info(char *fname, struct stat *buf) {
    // <<<
    char modestr[11];
    mode_to_letters(buf->st_mode, modestr);
    printf("%s ", modestr);
    printf("%-4d", (int)buf->st_nlink);
    printf("%-8s", uid_to_name(buf->st_uid));
    printf("%-8s", gid_to_name(buf->st_gid));
    printf("%-10ld", (long)buf->st_size);
    printf("%-.12s", 4 + ctime(&buf->st_mtim.tv_sec));
    printf(" %s\n", fname);
    // >>>
}
