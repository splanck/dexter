#include "streamer.h"
#include "heap.h"
#include "kheap.h"

struct disk_stream* disk_streamer_new(int disk_id) {
    struct disk* disk = disk_get(disk_id);
    if(!disk) return 0;

    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->disk = disk;
    streamer->pos = 0;

    return streamer;
}

int diskstreamer_seek(struct disk_stream* stream, int pos) {
    stream->pos = pos;

    return 0;
}
