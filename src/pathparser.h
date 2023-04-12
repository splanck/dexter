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

#endif