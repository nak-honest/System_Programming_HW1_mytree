struct Node {
    // unsigned long inodeNum;
    // long int devNum;
    // char *perm;
    // char *owner;
    // char *fileSize;
    char *fileName;
    long int offset;
    int order;
    int isFirst;
};
