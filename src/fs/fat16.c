#include "../fs/fat16.h"
#include "../fs/file.h"
#include "../fs/disk.h"
#include "../fs/streamer.h"
#include "../sys/status.h"
#include "../sys/config.h"
#include "../sys/kernel.h"
#include "../lib/string.h"
#include "../mem/heap.h"
#include "../mem/kheap.h"
#include "../mem/memory.h"
#include "../fs/fat_header.h"

struct filesystem fat16_fs =
{
    .resolve = fat16_resolve,
    .open = fat16_open
};

struct filesystem *fat16_init()
{
    strcpy(fat16_fs.name, "FAT16");
    return &fat16_fs;
}

static void fat16_init_private(struct disk *disk, struct fat_private *private)
{
    memset(private, 0, sizeof(struct fat_private));

    private->cluster_read_stream = disk_streamer_new(disk->id);
    private->fat_read_stream = disk_streamer_new(disk->id);
    private->directory_stream = disk_streamer_new(disk->id);
}

int fat16_sector_to_absolute(struct disk *disk, int sector)
{
    return sector * disk->sector_size;
}

int fat16_get_total_items_for_directory(struct disk *disk, uint32_t directory_start_sector)
{
    int r = 0;
    int i = 0;
    
    struct fat_directory_item item;
    struct fat_directory_item empty_item;

    memset(&empty_item, 0, sizeof(empty_item));

    struct fat_private *fat_private = disk->fs_private;
    struct disk_stream *stream = fat_private->directory_stream;

    int directory_start_pos = directory_start_sector * disk->sector_size;

    if (disk_streamer_seek(stream, directory_start_pos) != DEXTER_ALL_OK)
    {
        r = -EIO;
        goto out;
    }

    while (1)
    {
        if (disk_streamer_read(stream, &item, sizeof(item)) != DEXTER_ALL_OK)
        {
            r = -EIO;
            goto out;
        }

        if (item.filemame[0] == 0x00)
        {
            break;
        }

        if (item.filemame[0] == 0xE5)
        {
            continue;
        }

        i++;
    }

    r = i;
out:
    return r;
}

int fat16_get_root_directory(struct disk *disk, struct fat_private *fat_private, struct fat_directory *directory)
{
    int r = 0;

    struct fat_header *primary_header = &fat_private->header.primary_haeader;

    int root_dir_sector_pos = (primary_header->fat_copies * primary_header->sectors_per_fat) 
        + primary_header->reserved_sectors;

    int root_dir_entries = fat_private->header.primary_haeader.root_dir_entries;
    int root_dir_size = root_dir_entries * sizeof(struct fat_directory_item);
    int total_sectors = root_dir_size / disk->sector_size;

    if (root_dir_size % disk->sector_size)
    {
        total_sectors++;
    }

    int total_items = fat16_get_total_items_for_directory(disk, root_dir_sector_pos);

    struct fat_directory_item *dir = kzalloc(root_dir_size);

    if (!dir)
    {
        r = -ENOMEM;
        goto out;
    }

    struct disk_stream *stream = fat_private->directory_stream;

    if (disk_streamer_seek(stream, fat16_sector_to_absolute(disk, root_dir_sector_pos)) != DEXTER_ALL_OK)
    {
        r = -EIO;
        goto out;
    }

    if (disk_streamer_read(stream, dir, root_dir_size))
    {
        r = -EIO;
        goto out;
    }

    directory->item = dir;
    directory->total = total_items;
    directory->sector_pos = root_dir_sector_pos;
    directory->ending_sector_pos = root_dir_sector_pos + (root_dir_size / disk->sector_size);

out:
    return r;
}

void fat16_to_proper_string(char** out, const char* in)
{
    while(*in != 0x00 && *in != 0x20)
    {
        **out = *in;
        *out += 1;
        in += 1;
    }

    if(*in == 0x20)
    {
        **out = 0x00;
    }
}

void fat16_get_full_relative_filename(struct fat_directory_item* item, char* out, int max_len) 
{
    memset(out, 0x00, max_len);
    char *out_tmp = out;

    // Retrieve filename string from fat directory item.
    fat16_to_proper_string(&out_tmp, (const char*)item->filemame);

    // If filename has an extension, append extension to filename string.
    if(item->ext[0] != 0x00 && item->ext[0] != 0x20)
    {
        *out_tmp++ = '.';
        fat16_to_proper_string(&out_tmp, (const char*)item->ext);
    }
}

int fat16_get_first_cluster(struct fat_directory_item* item)
{
    return 0;
}

struct fat_directory *fat16_load_fat_directory(struct disk* disk, struct fat_directory_item* item)
{
    int r = 0;

    struct fat_directory* dir = 0;
    struct fat_private* fat_private = disk->fs_private;

    // If the fat directory item is not a subdirectory we can't load it, so we need to report invalid args.
    if(!(item->attribute & FAT_FILE_SUBDIRECTORY))
    {
        r = -EINVARG;
        goto out;
    }

    dir = kzalloc(sizeof(struct fat_directory));

    if(!dir)
    {
        r = -ENOMEM;
        goto out;
    }

out:
    return dir;
}

struct fat_directory_item *fat16_clone_directory_item(struct fat_directory_item* item, int size)
{
    struct fat_directory_item* item_copy = 0; kzalloc(size);
    
    // If requested allocation size is smaller than a fat_directory_item, abort operation.
    if(size < sizeof(struct fat_directory_item))
    {
        goto out;
    }

    item_copy = kzalloc(size);

    if(!item_copy)
    {
        goto out;
    }

    memcpy(item_copy, item, size);

out:
    return item_copy;
}

struct fat_item *fat16_new_fat_item_for_directory_item(struct disk* disk, struct fat_directory_item* item)
{
    struct fat_item* f_item = kzalloc(sizeof(struct fat_item));

    if(!f_item)
    {
        f_item = 0;
        
        goto out;
    }

    if(item->attribute & FAT_FILE_SUBDIRECTORY)
    {
        f_item->directory = fat16_load_fat_directory(disk, item);
        f_item->type = FAT_ITEM_TYPE_DIRECTORY;
        
        goto out;
    }

    f_item->type = FAT_ITEM_TYPE_FILE;
    f_item->item = fat16_clone_directory_item(item, sizeof(struct fat_directory_item));
out:
    return f_item;
}

struct fat_item *fat16_find_item_in_directory(struct disk *disk, struct fat_directory* dir, const char* name)
{
    struct fat_item* f_item = 0;
    char tmp_filename[DEXTER_MAX_PATH];

    for(int i = 0; i < dir->total; i++)
    {
        fat16_get_full_relative_filename(&dir->item[i], tmp_filename, sizeof(tmp_filename));

        if(istrncmp(tmp_filename, name, sizeof(tmp_filename)))
        {
            f_item = fat16_new_fat_item_for_directory_item(disk, &dir->item[i]);
        }
    }

    return f_item;
}

struct fat_item *fat16_get_directory_entry(struct disk *disk, struct path_part *path)
{
    struct fat_private* fat_private = disk->fs_private;
    struct fat_item* current_item = 0;
    struct fat_item* root_item = fat16_find_item_in_directory(disk, &fat_private->root_directory, path->part);

    if(!root_item)
    {
        goto out;
    }

out:
    return current_item;
}

int fat16_resolve(struct disk *disk)
{
    int r = 0;

    struct fat_private *fat_private = kzalloc(sizeof(struct fat_private));
    fat16_init_private(disk, fat_private);

    disk->fs_private = fat_private;
    disk->filesystem = &fat16_fs;

    struct disk_stream *stream = disk_streamer_new(disk->id);

    if (!stream)
    {
        r = -ENOMEM;
        goto out;
    }

    int x = disk_streamer_read(stream, &fat_private->header, sizeof(fat_private->header));

    if (x != DEXTER_ALL_OK)
    {
        r = -EIO;
        goto out;
    }

    if (fat_private->header.shared.extended_header.signature != 0x29)
    {
        r = -EFSNOTUS;
        goto out;
    }

    if (fat16_get_root_directory(disk, fat_private, &fat_private->root_directory) != DEXTER_ALL_OK)
    {
        r = -EIO;
        goto out;
    }

out:
    if (stream)
    {
        disk_streamer_close(stream);
    }

    if (r < 0)
    {
        kfree(fat_private);
        disk->fs_private = 0;
    }

    return r;
}

void *fat16_open(struct disk *disk, struct path_part *path, FILE_MODE mode)
{
    if (mode != FILE_MODE_READ)
    {
        return ERROR(-ERDONLY);
    }

    struct fat_file_descriptor *descriptor = 0;
    descriptor = kzalloc(sizeof(struct fat_file_descriptor));

    if (!descriptor)
    {
        return ERROR(-ENOMEM);
    }

    descriptor->item = fat16_get_directory_entry(disk, path);

    if (!descriptor->item)
    {
        return ERROR(-EIO);
    }

    descriptor->pos = 0;

    return descriptor;
}