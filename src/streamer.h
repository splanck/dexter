#ifndef STREAMER_H
#define STREAMER_H

#include "disk.h"

struct disk_stream {
    int pos;
    struct disk* disk;
};

struct disk_stream* disk_streamer_new(int disk_id);
int diskstreamer_seek(struct disk_stream* stream, int pos);

#endif