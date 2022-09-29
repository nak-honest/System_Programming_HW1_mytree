#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

long int GetDirEndOffset(const char *dirPath) {
    DIR *dirp;
    struct dirent *dirInfo;
    long int endOffset;

    if ((dirp = opendir(dirPath)) == NULL) {
        perror("opendir() error");
        exit(0);
    }

    while ((dirInfo = readdir(dirp)) != NULL) {
        if (dirInfo->d_name[0] != '.') {
            endOffset = dirInfo->d_off;
        }
    }

    closedir(dirp);

    return endOffset;
}
