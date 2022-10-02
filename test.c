#include "tree.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF_SIZE 256

int main(void) {
    struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));
    DIR *dirp;
    struct dirent *dirInfo;
    char *filePathName = "/home/nhlee98/DS/main";
    char *dirPathName = "/";
    char *perm = (char *)malloc(MAX_BUF_SIZE);
    char *fileSize = (char *)malloc(MAX_BUF_SIZE);
    off_t size = 1024 * 1224 * 1025;

    long int *offsetArr = (long int *)malloc(512);
    int entryNum = 0;
    int i = 0;
    entryNum = GetDirSortingOffset(dirPathName, offsetArr);

    GetFileSize(size, fileSize);
    printf("%s\n\n", fileSize);

    struct passwd *userInfo;

    if (stat(filePathName, fileInfo) == -1) {
        perror("stat() error");
        exit(0);
    }

    dirp = opendir(dirPathName);

    printf("entrynum : %d\n", entryNum);
    for (i = 0; i < entryNum; i++) {
        if (offsetArr[i] == 0) {
            rewinddir(dirp);
        } else {
            seekdir(dirp, (off_t)offsetArr[i]);
        }

        dirInfo = readdir(dirp);

        printf("inode No : %ld, Name : %s, offset : %ld\n ", dirInfo->d_ino,
               dirInfo->d_name, dirInfo->d_off);
    }

    free(fileInfo);
    free(perm);
    free(fileSize);
    free(offsetArr);
    closedir(dirp);
    return 0;
}
