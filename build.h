/** date = June 24th 2024 2:48 pm 
* This file contains the functions and macros to the automatic build system 
 * of the gtms (Graphical Turing Machine Simulator).
*/
#ifndef BUILD_H
#define BUILD_H

#include "src/include/base.h"


#include <assert.h>


// Build types

struct BuildCmd {
    String8 *items;
    u32 count;
};


function b32 needs_rebuild(); 
function void cmd_append(struct BuildCmd *cmd, String8 *str);
// NOTE(lucashdez) If something goes wrong
function void reset_files();
// NOTE(lucashdez): Runs the command
function void cmd_run(BuildCmd *cmd);


#ifndef REBUILD_YOURSELF
#    if defined(__clang__)
#        define REBUILD_YOURSELF(binary_path, source_path) "clang", "-o", binary_path, source_path
#    else
#        error "COMPILER NOT SUPPORTED"
#    endif
#endif

#define Stmnt(S) do{S}while(0)

#define GO_REBUILD_YOURSELF(argc, argv) \
Stmnt(const char *source_path = __FILE__; \
assert(argc >= 1); \
const char *binary_path = argv[0]; \
)



#endif //BUILD_H
