#include "streamer.h"
#include "heap.h"
#include "kheap.h"
#include "config.h"

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

int disk_streamer_close(struct disk_stream* stream) {
    kfree(stream);
}

int disk_streamer_read(struct disk_stream* stream, void* out, int total) {
    int sector = stream->pos / DEXTER_SECTOR_SIZE, offset = stream->pos % DEXTER_SECTOR_SIZE;
    int total_to_read = total > DEXTER_SECTOR_SIZE ? DEXTER_SECTOR_SIZE : total, res = 0;
    char buf[DEXTER_SECTOR_SIZE];

    res = disk_read_block(stream->disk, sector, 1, buf);
    
    if(res < 0) goto out;

    for(int x = 0; x < total; x++)
        *(char*)out++ = buf[offset + x];

    stream->pos += total_to_read;
    
    if(total > DEXTER_SECTOR_SIZE)
        res = disk_streamer_read(stream, out, total - DEXTER_SECTOR_SIZE);
out:
    return res;
}
