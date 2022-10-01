#include "node.h"
#include <sys/stat.h>
int GetFilePerm(const mode_t m, char *perm, const int bufSize);
int GetFileSize(off_t s, char *fileSize);
long int GetDirEndOffset(const char *dirPath);
void printMyTree(const char *parentPath, int depth, int *fileNum, int *dirNum);

void SeekPreviousOffset(const char *dirPath, struct Node *node, int numOfEntry,
                        long int *offsetArr);
int GetDirSortingOffset(const char *dirPath, long int *offsetArr);
int partition(struct Node *node, int low, int high);
void QuickSortNode(struct Node *node, int low, int high);
