#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Tree is implemented by recursive function(Depth First Search). */
void printTree(const char *parentPath, int depth) {
    struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));

    DIR *dirp;
    struct dirent *dirInfo;
    char *childPath = (char *)malloc(sizeof(char) * 256); // next file path

    int i;
    depth++; // tree's depth. When process visits sub directory, depth increases
             // of one.

    dirp = opendir(parentPath);

    while ((dirInfo = readdir(dirp)) != NULL) {
        /* If file's name starts with '.', the process do not visit. */
        if (dirInfo->d_name[0] == '.') {
            continue;
        }

        /* Update the path of the file that we will visit. */
        strcpy(childPath, parentPath);
        strcat(childPath, "/");
        strcat(childPath, dirInfo->d_name);

        if (stat(childPath, fileInfo) == -1) {
            perror("stat() error");
            exit(0);
        }

        /* If the file that we will visit is directory file, call printTree()
         * reculsively. */
        if (S_ISDIR(fileInfo->st_mode)) {
            for (i = 0; i < (depth - 1); i++) {
                printf("│   ");
            }
            printf("├──%s\n", dirInfo->d_name);

            printTree(childPath, depth);

        } else {
            for (i = 0; i < depth; i++) {
                printf("│   ");
            }
            printf("├──%s\n", dirInfo->d_name);
        }
    }

    free(fileInfo);
    free(childPath);
}

int main() {
    char filePath[512] = "/home";

    printf(".\n");

    printTree(filePath, 0);
    return 0;
}
