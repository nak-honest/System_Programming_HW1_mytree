#include <sys/stat.h>
int GetFilePerm(const mode_t m, char *perm, const int bufSize);
int GetFileSize(off_t s, char *fileSize);
long int GetDirEndOffset(const char *dirPath);
void printMyTree(const char *parentPath, int depth);
