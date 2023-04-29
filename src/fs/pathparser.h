#ifndef PATHPARSER_H
#define PATHPARSER_H

struct path_root 
{
    int drive_no;
    struct path_part* first;
};

struct path_part 
{
    const char* part;
    struct path_part* next;
};

static int pathparser_path_valid_format(const char* filename);
static struct path_root* pathparser_create_root(int drive_no);
struct path_part* pathparser_parse_path_part(struct path_part* last_part, const char** path);
static const char* pathparser_get_path_part(const char** path);
static int pathparser_get_drive_by_path(const char** path);
void pathparser_free(struct path_root* root);
struct path_root* pathparser_parse(const char* path, const char* current_directory_path);

#endif