/* ls1.c
 *
 * opendir
 * while(readdir)
 *   print d_name
 * close(readdir)
 *
 */

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void do_ls(char dirname[]) {
    DIR *dir_ptr;
    struct dirent *direntp;  // 每个实体

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
        return;
    } else {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            printf("%s\n", direntp->d_name);
        }

        closedir(dir_ptr);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_ls(".");
    } else {
        while (--argc) {
            printf("%s\n", *++argv);
            do_ls(*argv);
        }
    }

    return 0;
}
