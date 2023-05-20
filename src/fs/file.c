#include "fs/file.h"
#include "fs/disk.h"
#include "fs/fat16.h"
#include "sys/config.h"
#include "sys/status.h"
#include "sys/kernel.h"
#include "mem/memory.h"
#include "mem/kheap.h"
#include "lib/string.h"

struct filesystem* filesystems[DEXTER_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[DEXTER_MAX_FILE_DESCRIPTORS];

static struct filesystem** fs_get_free_filesystem()
{
    int i = 0;

    for (i = 0; i < DEXTER_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] == 0)
        {
            return &filesystems[i];
        }
    }

    return 0;
}

void fs_insert_filesystem(struct filesystem* filesystem)
{
    struct filesystem** fs;
    fs = fs_get_free_filesystem();

    if (!fs)
    {
        //print("Problem inserting filesystem"); 
        while(1) {}
    }

    *fs = filesystem;
}

static void fs_static_load()
{
    fs_insert_filesystem(fat16_init());
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

static int file_new_descriptor(struct file_descriptor **desc_out)
{
    int r = -ENOMEM;

    for (int i = 0; i < DEXTER_MAX_FILE_DESCRIPTORS; i++)
    {
        if (file_descriptors[i] == 0)
        {
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            // Descriptors start at 1
            desc->index = i + 1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            r = 0;

            break;
        }
    }

    return r;
}

static struct file_descriptor* file_get_descriptor(int fd)
{
    if (fd <= 0 || fd >= DEXTER_MAX_FILE_DESCRIPTORS)
    {
        return 0;
    }

    // Descriptors start at 1
    int index = fd - 1;

    return file_descriptors[index];
}

static void file_free_descriptor(struct file_descriptor* desc)
{
    file_descriptors[desc->index - 1] = 0x00;
    kfree(desc);
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

FILE_MODE file_get_mode_by_string(const char* str)
{
    FILE_MODE mode = FILE_MODE_INVALID;

    if (strncmp(str, "r", 1) == 0)
    {
        mode = FILE_MODE_READ;
    }
    else if(strncmp(str, "w", 1) == 0)
    {
        mode = FILE_MODE_WRITE;
    }
    else if(strncmp(str, "a", 1) == 0)
    {
        mode = FILE_MODE_APPEND;
    }

    return mode;
}

int fread(void* ptr, uint32_t size, uint32_t nmemb, int fd)
{
    int r = 0;

    if(size == 0 || nmemb == 0 || fd < 1)
    {
        r = -EINVARG;
        goto out;
    }

    struct file_descriptor* desc = file_get_descriptor(fd);

    if(!desc)
    {
        r = -EINVARG;
        goto out;
    }

    r = desc->filesystem->read(desc->disk, desc->private, size, nmemb, (char*)ptr);

out:
    return r;
}

int fclose(int fd)
{
    int r = 0;

    struct file_descriptor* desc = file_get_descriptor(fd);

    if(!desc)
    {
        r = -EIO;
        goto out;
    }

    r = desc->filesystem->close(desc->private);

    if(r == DEXTER_ALL_OK)
    {
        file_free_descriptor(desc);
    }

out:
    return r;
}

int fstat(int fd, struct file_stat* stat)
{
    int r = 0;

    struct file_descriptor* desc = file_get_descriptor(fd);

    if(!desc)
    {
        r = -EIO;
        goto out;
    }

    r = desc->filesystem->stat(desc->disk, desc->private, stat);
out:
    return r;
}

int fseek(int fd, int offset, FILE_SEEK_MODE whence)
{
    int r = 0;

    struct file_descriptor* desc = file_get_descriptor(fd);

    if(!fd)
    {
        r = -EIO;
        goto out;
    }

    r = desc->filesystem->seek(desc->private, offset, whence);

out:
    return r;
}

int fopen(const char* filename, const char* mode_str)
{
    int r = 0;
    struct path_root* root_path = pathparser_parse(filename, NULL);

    if (!root_path)
    {
        r = -EINVARG;
        goto out;
    }

    // We cannot have just a root path 0:/ 0:/test.txt
    if (!root_path->first)
    {
        r = -EINVARG;
        goto out;
    }

    // Ensure the disk we are reading from exists
    struct disk* disk = disk_get(root_path->drive_no);

    if (!disk)
    {
        r = -EIO;
        goto out;
    }

    if (!disk->filesystem)
    {
        r = -EIO;
        goto out;
    }

    FILE_MODE mode = file_get_mode_by_string(mode_str);

    if (mode == FILE_MODE_INVALID)
    {
        r = -EINVARG;
        goto out;
    }

    void* descriptor_private_data = disk->filesystem->open(disk, root_path->first, mode);

    if (ISERR(descriptor_private_data))
    {
        r = ERROR_I(descriptor_private_data);
        goto out;
    }

    struct file_descriptor* desc = 0;
    r = file_new_descriptor(&desc);

    if (r < 0)
    {
        goto out;
    }
    
    desc->filesystem = disk->filesystem;
    desc->private = descriptor_private_data;
    desc->disk = disk;

    r = desc->index;
out:
    // fopen shouldnt return negative values
    if (r < 0)
    {
        r = 0;
    }

    return r;
}

