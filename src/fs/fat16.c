#include "../fs/fat16.h"
#include "../fs/file.h"
#include "../fs/disk.h"
#include "../fs/streamer.h"
#include "../sys/status.h"
#include "../lib/string.h"
#include "../mem/heap.h"
#include "../mem/kheap.h"
#include "../mem/memory.h"

int fat16_resolve(struct disk* disk);
void* fat16_open(struct disk* disk, struct path_part* path, FILE_MODE mode);

struct fat_header_extended 
{
    uint8_t drive_number;
    uint8_t win_nt_bit;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_id_string[11];
    uint8_t system_id_string[8];
} __attribute__((packed));

struct fat_header 
{
    uint8_t short_jmp_ins[3];
    uint8_t oem_indentifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_copies;
    uint16_t root_dir_entries;
    uint16_t number_of_sectors;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t sectors_big;
} __attribute__((packed));

struct fat_h 
{
    struct fat_header primary_haeader;
    union fat_h_e {
        struct fat_header_extended extended_header;
    } shared;
};

struct fat_directory_item 
{
    uint8_t filemame[8];
    uint8_t ext[3];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t creation_time_tenths_of_a_second;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access;
    uint16_t high_16_bits_first_cluster;
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t low_16_bits_first_cluster;
    uint32_t filesize; 
} __attribute__((packed)); 

struct fat_directory 
{
    struct fat_directory_item* item;
    int total;
    int sector_pos;
    int ending_sector_pos;
};

struct fat_item 
{
    union 
    {
        struct fat_directory_item* item;
        struct fat_directory* directory;
    };

    FAT_ITEM_TYPE type;
};

struct fat_item_descriptor 
{
    struct fat_item* item;
    uint32_t pos;
};

struct fat_private 
{
    struct fat_h header;
    struct fat_directory directory;
    struct disk_stream* cluster_read_stream;
    struct disk_stream* fat_read_stream;
    struct disk_stream* directory_stream;
};

struct filesystem fat16_fs = 
{
    .resolve = fat16_resolve,
    .open = fat16_open
};

struct filesystem* fat16_init() 
{
    strcpy(fat16_fs.name, "FAT16");
    return &fat16_fs;
}

static void fat16_init_private(struct disk* disk, struct fat_private* private) 
{
    memset(private, 0, sizeof(struct fat_private));
    
    private->cluster_read_stream = disk_streamer_new(disk->id);
    private->fat_read_stream = disk_streamer_new(disk->id);
    private->directory_stream = disk_streamer_new(disk->id);
}

int fat16_sector_to_absolute(struct disk* disk, int sector) 
{
    return sector * disk->sector_size;
}

int fat16_get_total_items_for_directory(struct disk* disk, uint32_t directory_start_sector)
{
    struct fat_directory_item item;
    struct fat_directory_item empty_item;

    memset(&empty_item, 0, sizeof(empty_item));
    
    return 0;
}

int fat16_get_root_directory(struct disk* disk, struct fat_private* fat_private, struct fat_directory* directory) 
{
    int r = 0;

    struct fat_header* primary_header = &fat_private->header.primary_haeader;

    int root_dir_sector_pos = (primary_header->fat_copies * primary_header->sectors_per_fat)
        + primary_header->reserved_sectors;

    int root_dir_entries = fat_private->header.primary_haeader.root_dir_entries;
    int root_dir_size = root_dir_entries * sizeof(struct fat_directory_item);
    int total_sectors = root_dir_size / disk->sector_size;

    if(root_dir_size % disk->sector_size) 
    {
        total_sectors++;
    }

    int total_items = fat16_get_total_items_for_directory(fat_private, root_dir_sector_pos);

    struct fat_directory_item* dir = kzalloc(root_dir_size);

    if(!dir) 
    {
        r = -ENOMEM;
        goto out;
    }

    struct disk_stream* stream = fat_private->directory_stream;

    if(diskstreamer_seek(stream, fat16_sector_to_absolute(disk, root_dir_sector_pos)) != DEXTER_ALL_OK)
    {
        r = -EIO;
        goto out;
    }

    if(disk_streamer_read(stream, dir, root_dir_size))
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

int fat16_resolve(struct disk* disk) 
{
    int r = 0;

    struct fat_private* fat_private = kzalloc(sizeof(struct fat_private));
    fat16_init_private(disk, fat_private);

    struct disk_stream* stream = disk_streamer_new(disk->id);

    if(!stream) 
    {
        r = -ENOMEM;
        goto out;
    }

    int x = disk_streamer_read(stream, &fat_private->header, sizeof(fat_private->header));

    if(x != DEXTER_ALL_OK) 
    {
        r = -EIO;
        goto out;
    }

    if(fat_private->header.shared.extended_header.signature != 0x29) 
    {
        r = -EFSNOTUS;
        goto out;
    }

    if(fat16_get_root_directory(disk, &fat_private->root_directory) != DEXTER_ALL_OK) 
    {
        r = -EIO;
        goto out;
    }

out:
    return r;
}

void* fat16_open(struct disk* disk, struct path_part* path, FILE_MODE mode) 
{
    return 0;
}