#ifndef DISK_H
#define DISK_H

#include "../fs/file.h"

typedef unsigned int DEXTER_DISK_TYPE;

#define DEXTER_DISK_TYPE_PHYSICAL 0;

struct disk {
    DEXTER_DISK_TYPE type;
    int sector_size;
    struct filesystem* filesystem;
};

int disk_read_sectors(int lba, int total, void* buf);
void disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif