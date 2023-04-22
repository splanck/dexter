#ifndef FILE_H
#define FILE_H

#include "../fs/pathparser.h"
#include "../fs/disk.h"

typedef unsigned int FILE_SEEK_MODE;
enum {
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef unsigned int FILE_MODE;
enum {
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

typedef void*(*FS_OPEN_FUNCTION)(struct disk* disk, struct path_part* path, FILE_MODE mode);
typedef int*(*FS_RESOLVE_FUNCTION)(struct disk* disk);

struct filesystem {
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;
    char name[20];
};

struct file_descriptor {
    int index;
    struct filesystem* filesystem;
    void* private;
    struct disk* disk;
};

void fs_init();
int fopen(const char* filename, const char* mode);

#endif