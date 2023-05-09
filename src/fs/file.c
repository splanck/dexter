#include "../fs/file.h"
#include "../fs/fat16.h"
#include "../fs/disk.h"
#include "../sys/kernel.h"
#include "../sys/config.h"
#include "../lib/string.h"
#include "../mem/heap.h"
#include "../mem/kheap.h"
#include "../sys/status.h"
#include "../mem/memory.h"

struct filesystem* filesystems[DEXTER_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[DEXTER_MAX_FILE_DESCRIPTORS];

static struct filesystem** fs_get_free_filesystem() 
{
    for(int x = 0; x < DEXTER_MAX_FILESYSTEMS; x++) 
    {
        if(filesystems[x] == 0) 
        {
            return &filesystems[x];
        }
    }

    return 0;
}

static void fs_static_load() 
{
    fs_insert_filesystem(fat16_init());
}

static int file_new_descriptor(struct file_descriptor** desc_out) 
{
    int r = -ENOMEM;
    
    for(int x = 0; x < DEXTER_MAX_FILE_DESCRIPTORS; x++) 
    {
        if(file_descriptors[x]) 
        {
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            desc->index = x + 1;
            
            file_descriptors[x] = desc;
            *desc_out = desc;

            r = 0;
            break;
        }
    }

    return r;
}

static struct file_descriptor* file_get_descriptor(int fd) 
{
    if(fd <= 0 || fd >= DEXTER_MAX_FILE_DESCRIPTORS) 
    {
        return 0;
    }

    int index = fd - 1;

    return file_descriptors[index];
}

struct filesystem* fs_resolve(struct disk* disk)
{
    struct filesystem* fs = 0;

    for (int i = 0; i < DEXTER_MAX_FILESYSTEMS; i++) 
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0) 
        {
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}

void fs_insert_filesystem(struct filesystem* filesystem) 
{
    struct filesystem** fs;
    fs = fs_get_free_filesystem();

    if(!fs) 
    {
        kernel_panic();
    }

    *fs = filesystem;
}

void fs_load() 
{
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init() 
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

FILE_MODE file_get_mode_by_str(const char* str)
{
    FILE_MODE mode = FILE_MODE_INVALID;

    if(strncmp(str, "r", 1) == 0)
    {
        mode = FILE_MODE_READ;
    }
    else if(strncmp(str, "", 1) == 0) 
    {
        mode = FILE_MODE_WRITE;
    }
    else if(strncmp(str, "a", 1) == 0) 
    {
        mode = FILE_MODE_APPEND;
    }
    
    return mode;
}

int fopen(const char* filename, const char* mode) 
{
    int r = 0;
    struct path_root* root_path = pathparser_parse(filename, NULL);

    if(!root_path) 
    {
        r = -EINVARG;
        goto out;
    }

    if(!root_path->first)
    {
        r = -EINVARG;
        goto out;
    }

    struct disk* disk = disk_get(root_path->drive_no);

    if(!disk)
    {
        r = -EIO;
        goto out;
    }

    if(!disk->filesystem)
    {
        r = -EIO;
        goto out;
    }
out:
    return r;
}

