#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *GetFilePerm(const struct stat *fileInfo) {
    char *perm;
    perm = (char *)malloc(sizeof(char) * 11);
    memset(perm, '\0', sizeof(char) * 11);

    if (S_ISREG(fileInfo->st_mode)) {
        perm[0] = '-';
    } else if (S_ISDIR(fileInfo->st_mode)) {
        perm[0] = 'd';
    } else if (S_ISCHR(fileInfo->st_mode)) {
        perm[0] = 'c';
    } else if (S_ISBLK(fileInfo->st_mode)) {
        perm[0] = 'b';
    } else if (S_ISFIFO(fileInfo->st_mode)) {
        perm[0] = 'p';
    } else if (S_ISLNK(fileInfo->st_mode)) {
        perm[0] = 'l';
    } else if (S_ISSOCK(fileInfo->st_mode)) {
        perm[0] = 's';
    }

    if (S_IRUSR & fileInfo->st_mode) {
        perm[1] = 'r';
    } else {
        perm[1] = '-';
    }
    if (S_IWUSR & fileInfo->st_mode) {
        perm[2] = 'w';
    } else {
        perm[2] = '-';
    }
    if (S_IXUSR & fileInfo->st_mode) {
        perm[3] = 'x';
    } else {
        perm[3] = '-';
    }
    if (S_IRGRP & fileInfo->st_mode) {
        perm[4] = 'r';
    } else {
        perm[4] = '-';
    }
    if (S_IWGRP & fileInfo->st_mode) {
        perm[5] = 'w';
    } else {
        perm[5] = '-';
    }
    if (S_IXGRP & fileInfo->st_mode) {
        perm[6] = 'x';
    } else {
        perm[6] = '-';
    }
    if (S_IROTH & fileInfo->st_mode) {
        perm[7] = 'r';
    } else {
        perm[7] = '-';
    }
    if (S_IWOTH & fileInfo->st_mode) {
        perm[8] = 'w';
    } else {
        perm[8] = '-';
    }
    if (S_IXOTH & fileInfo->st_mode) {
        perm[9] = 'x';
    } else {
        perm[9] = '-';
    }

    return perm;
}
