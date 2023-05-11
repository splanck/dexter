#ifndef FAT16_H
#define FAT16_H

#include <stdint.h>
#include "../fs/fat_header.h"

#define FAT_ITEM_TYPE_DIRECTORY 0
#define FAT_ITEM_TYPE_FILE 1

#define DEXTER_FAT16_SIGNATURE 0x29
#define DEXTER_FAT16_FAT_ENTRY_SIZE 0x02
#define DEXTER_FAT16_BAD_SECTOR 0xFF7
#define DEXTER_FAT16_UNUSED 0x00

#define FAT_FILE_READ_ONLY 0x01
#define FAT_FILE_HIDDEN 0x02
#define FAT_FILE_SYSTEM 0x04
#define FAT_FILE_VOLUME_LABEL 0x08
#define FAT_FILE_SUBDIRECTORY 0x10
#define FAT_FILE_ARCHIVED 0x20
#define FAT_FILE_DEVICE 0x40
#define FAT_FILE_RESERVED 0x80


struct filesystem* fat16_init();
static void fat16_init_private(struct disk *disk, struct fat_private *private);
int fat16_sector_to_absolute(struct disk *disk, int sector);
int fat16_get_total_items_for_directory(struct disk *disk, uint32_t directory_start_sector);
int fat16_get_root_directory(struct disk *disk, struct fat_private *fat_private, struct fat_directory *directory);
void fat16_to_proper_string(char** out, const char* in);
struct fat_item *fat16_get_directory_entry(struct disk* disk, struct path_part *path);
void fat16_get_full_relative_filename(struct fat_directory_item* item, char* out, int max_len);
struct fat_directory *fat16_load_fat_directory(struct disk* disk, struct fat_directory_item* item);
struct fat_directory_item *fat16_clone_directory_item(struct fat_directory_item* item, int size);
struct fat_item *fat16_new_fat_item_for_directory_item(struct disk* disk, struct fat_directory_item* item);
struct fat_item *fat16_find_item_in_directory(struct disk* disk, struct fat_directory* dir, const char* name);
int fat16_resolve(struct disk *disk);
void *fat16_open(struct disk *disk, struct path_part *path, FILE_MODE mode);

#endif