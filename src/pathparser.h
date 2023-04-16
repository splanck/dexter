#ifndef PATHPARSER_H
#define PATHPARSER_H

struct path_root {
    int drive_no;
    struct pathpart* first;
};

struct path_part {
    const char* part;
    struct pathpart* next;
};

static int pathparser_path_valid_format(const char* filename);
static struct path_root* pathparser_create_root(int drive_no);
static const char* pathparser_get_path_part(const char** path);
static int pathparser_get_drive_by_path(const char** path);

#endif