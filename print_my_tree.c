#include "tree.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF_SIZE 256

char branchInfo[512];

/* Tree is implemented by recursive function(Depth First Search). */
void printMyTree(const char *parentPath, int depth) {
    DIR *dirp;
    struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));
    struct passwd *userInfo;
    struct dirent *dirInfo;

    char *childPath = (char *)malloc(MAX_BUF_SIZE); // next file path
    char *perm = (char *)malloc(MAX_BUF_SIZE);      // file permission
    char *fileSize = (char *)malloc(MAX_BUF_SIZE);  // file size(unit: K, M, G)

    int i;

    /* When process visits sub directory, tree's depth increases of one. */
    depth++;

    branchInfo[depth - 1] = 'Y';

    if ((dirp = opendir(parentPath)) == NULL) {
        perror("opendir() error");
        exit(0);
    }

    while ((dirInfo = readdir(dirp)) != NULL) {
        /* If file's name starts with '.', the process do not visit. */
        if (dirInfo->d_name[0] == '.') {
            continue;
        }

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

        if (GetDirEndOffset(parentPath) == dirInfo->d_off) {
            branchInfo[depth - 1] = 'N';
        }

        for (i = 0; i < (depth - 1); i++) {
            if (branchInfo[i] == 'Y') {
                printf("│   ");
            } else if (branchInfo[i] == 'N') {
                printf("    ");
            }
        }
        /* If the file that we will visit is directory file, call printTree()
         * reculsively. */
        if (S_ISDIR(fileInfo->st_mode)) {
            if (branchInfo[depth - 1] == 'N') {
                printf("└── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            } else {
                printf("├── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            }

            printMyTree(childPath, depth);
        } else {
            if (branchInfo[depth - 1] == 'N') {
                printf("└── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            } else {
                printf("├── [%8lu %4ld %s %s %8s]  %s\n", fileInfo->st_ino,
                       fileInfo->st_dev, perm, userInfo->pw_name, fileSize,
                       dirInfo->d_name);
            }
        }
    }

    closedir(dirp);
    free(fileInfo);
    free(childPath);
    free(perm);
    free(fileSize);
}
