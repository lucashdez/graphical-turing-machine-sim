/* date = June 28th 2024 4:37 pm */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

struct FM_FileInfo {
    i64 creation_time;
    i64 mod_time;
    i64 stat_change;
};

struct FM_File{
    struct Arena arena;
    void* handle;
    int fd;
    String8 path;
	String8 contents;
    struct FM_FileInfo info;
	u64 size;
};


static struct  FM_File fm_open_file(String8 path, String8 mode);
static void fm_read_file(struct FM_File *file); 
static u64 fm_file_size(struct FM_File *file);


#endif //FILE_MANAGER_H
