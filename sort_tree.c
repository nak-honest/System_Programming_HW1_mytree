#include "node.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int partition(struct Node *node, int low, int high) {
    int i, j, pivotPoint;
    char *pivotitem;
    char *tempName;
    long int tempOffset;

    pivotitem = (node + low)->fileName;

    j = low;
    for (i = low + 1; i <= high; i++) {
        if (strcmp(pivotitem, (node + i)->fileName) > 0) {
            j++;

            tempName = (node + i)->fileName;
            tempOffset = (node + i)->offset;

            (node + i)->fileName = (node + j)->fileName;
            (node + i)->offset = (node + j)->offset;

            (node + j)->fileName = tempName;
            (node + j)->offset = tempOffset;
        }
    }

    pivotPoint = j;

    tempName = (node + low)->fileName;
    tempOffset = (node + low)->offset;

    (node + low)->fileName = (node + pivotPoint)->fileName;
    (node + low)->offset = (node + pivotPoint)->offset;

    (node + pivotPoint)->fileName = tempName;
    (node + pivotPoint)->offset = tempOffset;

    return pivotPoint;
}

void QuickSortNode(struct Node *node, int low, int high) {
    int pivotPoint;

    if (high > low) {
        pivotPoint = partition(node, low, high);
        QuickSortNode(node, low, pivotPoint - 1);
        QuickSortNode(node, pivotPoint + 1, high);
    }
}

void SeekPreviousOffset(const char *dirPath, struct Node *node, int numOfEntry,
                        long int *offsetArr) {
    DIR *dirp;
    struct dirent *dirInfo;
    int i = 0;
    int offsetOrder = 0;

    if ((dirp = opendir(dirPath)) == NULL) {
        perror("opendir() error");
        exit(0);
    }

    while (((dirInfo = readdir(dirp)) != NULL) && (i < numOfEntry)) {
        if ((node + i)->offset == dirInfo->d_off) {
            (node + i)->order = offsetOrder;

            if (offsetOrder == 0) {
                (node + i)->isFirst = 1;
            } else {
                (node + i)->isFirst = 0;
            }

            rewinddir(dirp);
            offsetOrder = 0;
            i++;

            continue;
        }
        offsetOrder++;
    }

    rewinddir(dirp);
    offsetOrder = 1;
    i = 0;

    while (((dirInfo = readdir(dirp)) != NULL) && (i < numOfEntry)) {
        if ((node + i)->isFirst) {
            rewinddir(dirp);
            offsetOrder = 1;
            offsetArr[i] = 0;
            i++;
            continue;
        } else if (offsetOrder == (node + i)->order) {
            offsetArr[i] = dirInfo->d_off;

            rewinddir(dirp);
            offsetOrder = 1;
            i++;

            continue;
        }

        offsetOrder++;
    }
}
int GetDirSortingOffset(const char *dirPath, long int *offsetArr) {
    DIR *dirp;
    struct dirent *dirInfo;
    struct Node node[4096];

    int i;
    int numOfEntry = 0;

    if ((dirp = opendir(dirPath)) == NULL) {
        perror("opendir() error");
        exit(0);
    }

    while ((dirInfo = readdir(dirp)) != NULL) {
        if (dirInfo->d_name[0] != '.') {
            (node + numOfEntry)->fileName = dirInfo->d_name;
            (node + numOfEntry)->offset = dirInfo->d_off;
            numOfEntry++;
        }
    }

    QuickSortNode(node, 0, numOfEntry - 1);

    for (i = 0; i < numOfEntry; i++) {
        offsetArr[i] = (node + i)->offset;
    }

    SeekPreviousOffset(dirPath, node, numOfEntry, offsetArr);

    closedir(dirp);

    return numOfEntry;
}
