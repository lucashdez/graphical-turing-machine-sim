/** date = June 24th 2024 2:48 pm 
* This file contains the functions and macros to the automatic build system 
 * of the gtms (Graphical Turing Machine Simulator).
*/
#ifndef BUILD_H
#define BUILD_H

#include "src/include/base.h"
#include <stdio.h>

///////////////////////////
// SO THINGS

//   WINDOWS
#ifdef _WIN32
#  include "src/include/win32.h"
#else
#endif


// Build types

struct BuildCmd {
    String8 *items;
    u32 count;
};




function b32 needs_rebuild(String8 output_path, String8 input_path); 
function void cmd_append(struct BuildCmd *cmd, String8 str);
// NOTE(lucashdez) If something goes wrong
function void reset_files();
// NOTE(lucashdez): Runs the command
function void cmd_run(struct BuildCmd *cmd);

#define BUILDER_INFO "[INFO] "
#define BUILDER_ERROR "[ERROR] "

#define BUILDER_LOG(info, fmt, ...) printf(GlueStr(GlueStr(info, fmt), "\n"), __VA_ARGS__)

#ifndef REBUILD_YOURSELF
#    if defined(__clang__)
#        define REBUILD_YOURSELF(binary_path, source_path) "clang", "-o", binary_path, source_path
#    else
#        error "COMPILER NOT SUPPORTED"
#    endif
#endif

#include <assert.h>

#define GO_REBUILD_YOURSELF(argc, argv) \
Statement(const char *source_path = __FILE__; \
assert(argc >= 1); \
const char *binary_path = argv[0]; \
if (needs_rebuild(string_u8_litexpr(binary_path), string_u8_litexpr(source_path))) {\
BUILDER_LOG(BUILDER_INFO, "Rebuilding...");\
} \
)



#endif //BUILD_H