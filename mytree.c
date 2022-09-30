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
    if (getcwd(filePath, 4096) == NULL) {
        perror("getcwd error");
        exit(-1);
    }

    printf(".\n");
    // char tfilePath[4096] = "/dev";
    printMyTree(filePath, 0);

    return 0;
}
