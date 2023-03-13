#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
void show_info(char *fname, struct stat *buf);

int main(int argc, char *argv[]) {
    struct stat infobuf;
    if (argc > 1) {
        if (stat(argv[1], &infobuf) != -1) {
            show_info(argv[1], &infobuf);
        } else {
            perror(argv[1]);
        }
    }

    return 1;
}

void show_info(char *fname, struct stat *buf) {
    // 显示文件信息
    printf("\tmode: %o\n", buf->st_mode);
    printf("\tlinks: %d\n", buf->st_nlink);
    printf("\tuser: %d\n", buf->st_uid);
    printf("\tgroup: %d\n", buf->st_gid);
    printf("\tsize: %d\n", buf->st_size);
    printf("\tmodtime: %d\n", buf->st_mtim);
    printf("\tfilename: %s\n", fname);
}
