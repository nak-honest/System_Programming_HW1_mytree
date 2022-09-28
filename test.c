#include "tree.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));
    DIR *dirp;
    struct dirent *dirInfo;
    char *filePathName = "/home/nhlee98/DS/main";
    char *dirPathName = "/home/nhlee98";
    char *perm;

    struct passwd *userInfo;

    if (stat(filePathName, fileInfo) == -1) {
        perror("stat() error");
        exit(0);
    }

    dirp = opendir(dirPathName);

    while ((dirInfo = readdir(dirp)) != NULL) {
        printf("inode No : %ld, Name : %s\n ", dirInfo->d_ino, dirInfo->d_name);
    }

    userInfo = getpwuid(fileInfo->st_uid);
    perm = GetFilePerm(fileInfo);

    printf(
        "a.txt : inode No : %8ld, dev No : %ld, user : %s, size : %8ld, %s\n",
        fileInfo->st_ino, fileInfo->st_dev, userInfo->pw_name,
        fileInfo->st_size, perm);

    free(perm);
    closedir(dirp);
    return 0;
}
