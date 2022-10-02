#include "tree.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    char filePath[4096];
    int fileNum = 0;
    int dirNum = 0;
    /* Get current working directory */
    if (getcwd(filePath, 4096) == NULL) {
        perror("getcwd error");
        exit(-1);
    }

    printf(".\n");
    printMyTree(filePath, 0, &fileNum, &dirNum);
    printf("\n%d directories, %d files\n", dirNum, fileNum);

    return 0;
}
