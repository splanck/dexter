#include "pathparser.h"
#include "string.h"
#include "config.h"
#include "memory.h"
#include "kernel.h"
#include "heap.h"
#include "kheap.h"

static int path_valid_format(const char* filename) {
    int len = strnlen(filename, DEXTER_MAX_PATH);

    return (len >= 3 && isdigit(filename[0]) && memcmp((void*)&filename[1], ":/", 2));
}
