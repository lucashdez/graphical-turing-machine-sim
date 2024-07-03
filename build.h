/** date = June 24th 2024 2:48 pm 
* This file contains the functions and macros to the automatic build system 
 * of the gtms (Graphical Turing Machine Simulator).
*/
#ifndef BUILD_H
#define BUILD_H

#include "base/base_include.h"
#include <stdio.h>

///////////////////////////
// SO THINGS

// TODO Create a struct that contains the file information for future uses
//   WINDOWS
#ifdef _WIN32
#  include "win32.h"
#  include "src/win32.c"
#else
#  include "linux.h"
#endif


// Build types

struct StrNode {
    String8 str;
    struct StrNode *next;
};

struct StrList {
    struct Arena arena;
    struct StrNode* first;
    struct StrNode* last;
};

struct BuildCmd {
    struct Arena arena;
    struct StrList list;
    u32 count;
};


function b32 bs_needs_rebuild(String8 output_path, String8 input_path); 
function void bs_cmd_append(struct BuildCmd *cmd, String8 str);
function i32 bs_rename_file(String8 old_path, String8 new_path);
// NOTE(lucashdez) If something goes wrong
function void bs_reset_files();
// NOTE(lucashdez): Runs the command
function String8* bs_cmd_construct_command(struct Arena *arena, struct BuildCmd *cmd);
function void bs_cmd_run(struct BuildCmd *cmd);
function i32 bs_compare_file_time(struct FM_File* a, struct FM_File* b);
function void bs_get_builder_command(struct BuildCmd *cmd);
function i32 bs_run_command(String8* command); 

#define BUILDER_INFO "[INFO] "
#define BUILDER_ERROR "[ERROR] "

#define BUILDER_LOG(info, fmt) printf(GlueStr(GlueStr(info, fmt), "\n"))
#define BUILDER_LOG_ARGS(info, fmt, ...) printf(GlueStr(GlueStr(info, fmt), "\n"), __VA_ARGS__)

#ifndef REBUILD_YOURSELF
#    if defined(__clang__)
#        define REBUILD_YOURSELF(binary_path, source_path) "clang", "-o", binary_path, source_path
#    else
#        error "COMPILER NOT SUPPORTED"
#    endif
#endif

#include <assert.h>

#define GO_REBUILD_YOURSELF(argc, argv)                                                                \
Statement(const char *source_path = __FILE__;                                                      \
assert(argc >= 1);                                                                       \
const char *binary_path = argv[0];                                                       \
if (bs_needs_rebuild(string_u8_litexpr(binary_path), string_u8_litexpr(source_path))) {  \
if(!bs_rename_file(string_u8_litexpr(binary_path), string_u8_litexpr("build.old"))) {\
bs_reset_files(); /* Reset the modified files */                                 \
exit(1); /* Here we cancel everithing */                                         \
}                                                                                    \
BUILDER_LOG(BUILDER_INFO, "Rebuilding...");                                          \
struct BuildCmd cmd = {0};                                                           \
cmd.arena = mm_scratch_arena();                                                      \
cmd.list.arena = mm_scratch_arena();                                                 \
bs_get_builder_command(&cmd);                                                        \
String8* command = bs_cmd_construct_command(&cmd.arena, &cmd);                       \
BUILDER_LOG_ARGS(BUILDER_INFO, "Final command:\n\t%s", command->str);                \
BUILDER_LOG(BUILDER_INFO, "EXECUTING...");                                           \
if (!bs_run_command(command)) {                                                      \
bs_reset_files();                                                                \
exit(1);                                                                         \
}                                                                                    \
BUILDER_LOG(BUILDER_INFO, "Succeded!");                                              \
BUILDER_LOG(BUILDER_INFO, "Try to run again...");                                    \
exit(0);                                                                             \
}                                                                                        \
)



#endif //BUILD_H
