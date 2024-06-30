/* date = June 27th 2024 2:09 pm */

#ifndef LINUX_H
#define LINUX_H

//typedef i64 blkcnt_t;

struct Timespec {
    i64 tv_sec;
};

struct Stat {
    dev_t st_dev;
    ino_t st_ino;
    mode_t st_mode;
    nlink_t st_nlink;
    uid_t st_uid;
    gid_t st_gid;
    dev_t st_rdev;
    off_t st_size;
    blksize_t st_blksize;
    blkcnt_t st_blocks;
    
    struct Timespec st_atim;
    struct Timespec st_mtim;
    struct Timespec st_ctim;
    
#define st_atime st_atim.tv_sec 
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
};

// Functions
extern int stat(const char* restrict pathname, struct Stat *restrict statbuf);

#endif //LINUX_H
