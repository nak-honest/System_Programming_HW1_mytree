#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Get string of file's type and permission. */
int GetFilePerm(const mode_t m, char *perm, const int bufSize) {
    if (bufSize < 11) {
        return -1;
    }

    memset(perm, '\0', bufSize);

    /* file's type */
    if (S_ISREG(m)) {
        perm[0] = '-';
    } else if (S_ISDIR(m)) {
        perm[0] = 'd';
    } else if (S_ISCHR(m)) {
        perm[0] = 'c';
    } else if (S_ISBLK(m)) {
        perm[0] = 'b';
    } else if (S_ISFIFO(m)) {
        perm[0] = 'p';
    } else if (S_ISLNK(m)) {
        perm[0] = 'l';
    } else if (S_ISSOCK(m)) {
        perm[0] = 's';
    }

    /* file's permission */
    if (S_IRUSR & m) {
        perm[1] = 'r';
    } else {
        perm[1] = '-';
    }
    if (S_IWUSR & m) {
        perm[2] = 'w';
    } else {
        perm[2] = '-';
    }
    if (S_IXUSR & m) {
        perm[3] = 'x';
    } else {
        perm[3] = '-';
    }
    if (S_IRGRP & m) {
        perm[4] = 'r';
    } else {
        perm[4] = '-';
    }
    if (S_IWGRP & m) {
        perm[5] = 'w';
    } else {
        perm[5] = '-';
    }
    if (S_IXGRP & m) {
        perm[6] = 'x';
    } else {
        perm[6] = '-';
    }
    if (S_IROTH & m) {
        perm[7] = 'r';
    } else {
        perm[7] = '-';
    }
    if (S_IWOTH & m) {
        perm[8] = 'w';
    } else {
        perm[8] = '-';
    }
    if (S_IXOTH & m) {
        perm[9] = 'x';
    } else {
        perm[9] = '-';
    }

    return 0;
}

/* Get string of file's size. */
int GetFileSize(off_t s, char *fileSize) {
    double size = (double)s;
    int numOfModulo = 0;

    if (size < 1024) {
        sprintf(fileSize, "%d", (int)size);
        return 1;
    }

    /* If file's size >= 1024, represent the size by unit(K, M, G). */
    while (size >= 1024) {
        size /= (double)1024;
        numOfModulo++;
    }
    sprintf(fileSize, "%.1lf", size);

    switch (numOfModulo) {
    case 1:
        strcat(fileSize, "K\0");
        break;
    case 2:
        strcat(fileSize, "M\0");
        break;
    case 3:
        strcat(fileSize, "G\0");
        break;
    default:
        return -1;
    }
    return 0;
}
