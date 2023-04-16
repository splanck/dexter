#include "pathparser.h"
#include "string.h"
#include "config.h"
#include "memory.h"
#include "kernel.h"
#include "heap.h"
#include "kheap.h"
#include "status.h"

static int pathparser_path_valid_format(const char* filename) {
    int len = strnlen(filename, DEXTER_MAX_PATH);

    return (len >= 3 && isdigit(filename[0]) && memcmp((void*)&filename[1], ":/", 2));
}

static struct path_root* pathparser_create_root(int drive_no) {
    struct path_root* path_r = kzalloc(sizeof(struct path_root));

    path_r->drive_no = drive_no;
    path_r->first = 0; 

    return path_r;
}

static const char* pathparser_get_path_part(const char** path) {
    char* result_path_part = kzalloc(DEXTER_MAX_PATH);

    int i = 0;

    while(**path != '/' && **path != 0x00) {
        result_path_part[i] = **path;
        *path += 1;
        i++;
    };

    if(**path == '/')
        *path += 1;

    if(i == 0) {
        kfree(result_path_part);
        result_path_part = 0;
    }

    return result_path_part;
}

static int pathparser_get_drive_by_path(const char** path) {
    if(!pathparser_path_valid_format(*path))
        return -EBADPATH;

    int drive_no = tonumericdigit(*path[0]);
    *path += 3;

    return drive_no;
}
