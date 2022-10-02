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

/* If the file is last at each directory in depth i, isNotLastFile[i] is 'N'.
This value changes continue when the function is called reculsively. */
char isNotLastFile[MAX_BUF_SIZE];

/* Tree is implemented by recursive function(Depth First Search). */
void printMyTree(const char *parentPath, int depth, int *fileNum, int *dirNum) {
    DIR *dirp;
    struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));
    struct passwd *userInfo;
    struct dirent *dirInfo;

    char *childPath = (char *)malloc(MAX_BUF_SIZE); // next file path
    char *perm = (char *)malloc(MAX_BUF_SIZE);      // file permission
    char *fileSize = (char *)malloc(MAX_BUF_SIZE);  // file size(unit: K, M, G)
    long int *offsetArr = (long int *)malloc(
        MAX_BUF_SIZE); // parentPath directory entry offset sorted by Alphbet

    int entryNum = 0; // parent directory's entry number

    /* Sort entry's offset by Alphabet and assign previous offset. Because when
    seekdir(dirp, loc) is called, readdir() read next offset of loc. */
    entryNum = GetDirSortingOffset(parentPath, offsetArr);

    int i, j;

    /* When process visits sub directory, tree's depth increases of one. */
    depth++;

    /* When printMyTree() is called, we visit new directory(parentPath). So, we
    inintialize the information of last file in this parentPath directory. */
    isNotLastFile[depth - 1] = 'Y';

    if ((dirp = opendir(parentPath)) == NULL) {
        perror("opendir() error");
        exit(0);
    }

    /* print all files and directories in this parentPath directory. */
    for (i = 0; i < entryNum; i++) {
        if (offsetArr[i] == 0) {
            /* If offsetArr[i] is 0, (i+1)th file is in first offset entry. */
            rewinddir(dirp);
        } else {
            /* offsetArr[i] has (i+1)th file's directory entry offset. */
            seekdir(dirp, (off_t)offsetArr[i]);
        }

        dirInfo = readdir(dirp);

        /* Update the path of (i+1)th file. */
        strcpy(childPath, parentPath);
        strcat(childPath, "/");
        strcat(childPath, dirInfo->d_name);

        /* Update the stat of file. */
        if (lstat(childPath, fileInfo) == -1) {
            perror("stat() error");
            exit(1);
        }

        /* Update the (i+1)th file's user information. */
        if ((userInfo = getpwuid(fileInfo->st_uid)) == NULL) {
            perror("getpwuid() error");
            exit(2);
        }

        /* Update the permission of file. */
        if ((GetFilePerm(fileInfo->st_mode, perm, MAX_BUF_SIZE)) < 0) {
            perror("GetFilePerm() error");
            exit(3);
        }

        /* Update the size of file */
        if ((GetFileSize(fileInfo->st_size, fileSize)) < 0) {
            perror("GetFilePerm() error");
            exit(4);
        }

        /* If this (i+1)th file is parentPath directory's last file, tree's
        branch is closed. */
        if (i == entryNum - 1) {
            isNotLastFile[depth - 1] = 'N';
        }

        /* If depth j's directory that includes this childPath directory is
        last file, print the space. Otherwise, print '│'. */
        for (j = 0; j < (depth - 1); j++) {
            if (isNotLastFile[j] == 'Y') {
                printf("│   ");
            } else if (isNotLastFile[j] == 'N') {
                printf("    ");
            }
        }

        /* Print (i+1)th file's information. If this (i+1)th file is directory
        file, call printTree() reculsively. */
        if (S_ISDIR(fileInfo->st_mode)) {
            if (isNotLastFile[depth - 1] == 'N') {
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
            if (isNotLastFile[depth - 1] == 'N') {
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
