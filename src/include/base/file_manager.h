/* date = June 28th 2024 4:37 pm */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

struct FM_FileInfo {
    i64 creation_time;
    i64 mod_time;
    i64 stat_change;
};

struct FM_File{
    struct Arena* arena;
    void* handle;
    int fd;
    String8 path;
    struct FM_FileInfo info;
};

#endif //FILE_MANAGER_H
