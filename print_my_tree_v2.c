#include "tree.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF_SIZE 4096

char isThatLastFile[MAX_BUF_SIZE]; //

/* Tree is implemented by recursive function(Depth First Search). */
void printMyTree(const char *parentPath, int depth, int *fileNum, int *dirNum) {
    DIR *dirp;
    struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));
    struct passwd *userInfo;
    struct dirent *dirInfo;

    char *childPath = (char *)malloc(MAX_BUF_SIZE); // next file path
    char *perm = (char *)malloc(MAX_BUF_SIZE);      // file permission
    char *fileSize = (char *)malloc(MAX_BUF_SIZE);  // file size(unit: K, M, G)
    long int *offsetArr = (long int *)malloc(MAX_BUF_SIZE);

    int entryNum = 0;
    entryNum = GetDirSortingOffset(parentPath, offsetArr);

    int i, j;

    /* When process visits sub directory, tree's depth increases of one. */
    depth++;

    isThatLastFile[depth - 1] = 'Y';

    if ((dirp = opendir(parentPath)) == NULL) {
        perror("opendir() error");
        exit(0);
    }

    for (i = 0; i < entryNum; i++) {
        /* If file's name starts with '.', the process do not visit. */

        if (offsetArr[i] == 0) {
            rewinddir(dirp);
        } else {
            seekdir(dirp, (off_t)offsetArr[i]);
        }

        dirInfo = readdir(dirp);

        /* Update the path of the file that we will visit. */
        strcpy(childPath, parentPath);
        strcat(childPath, "/");
        strcat(childPath, dirInfo->d_name);

        if (lstat(childPath, fileInfo) == -1) {
            perror("stat() error");
            exit(1);
        }

        userInfo = getpwuid(fileInfo->st_uid);

        /* Update the permission of file. */
        if ((GetFilePerm(fileInfo->st_mode, perm, MAX_BUF_SIZE)) < 0) {
            perror("GetFilePerm() error");
            exit(2);
        }

        /* Update the size of file */
        if ((GetFileSize(fileInfo->st_size, fileSize)) < 0) {
            perror("GetFilePerm() error");
            exit(3);
        }

        if (i == entryNum - 1) {
            isThatLastFile[depth - 1] = 'N';
        }

        for (j = 0; j < (depth - 1); j++) {
            if (isThatLastFile[j] == 'Y') {
                printf("│   ");
            } else if (isThatLastFile[j] == 'N') {
                printf("    ");
            }
        }
        /* If the file that we will visit is directory file, call printTree()
         * reculsively. */
        if (S_ISDIR(fileInfo->st_mode)) {
            if (isThatLastFile[depth - 1] == 'N') {
                printf("└── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            } else {
                printf("├── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            }
            (*dirNum)++;
            printMyTree(childPath, depth, fileNum, dirNum);
        } else {
            if (isThatLastFile[depth - 1] == 'N') {
                printf("└── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            } else {
                printf("├── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            }
            (*fileNum)++;
        }
    }

    closedir(dirp);
    free(fileInfo);
    free(childPath);
    free(perm);
    free(fileSize);
    free(offsetArr);
}
