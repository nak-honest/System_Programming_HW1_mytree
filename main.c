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
    char filePath[512];
    if (getcwd(filePath, 512) == NULL) {
        perror("getcwd error");
        exit(-1);
    }

    printf(".\n");
    char tfilePath[512] = "/home";
    printMyTree(tfilePath, 0);

    return 0;
}
