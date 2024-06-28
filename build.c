#include "build.h"
#include <stdlib.h>

internal FILETIME
bs_get_filetime_from_path(String8 path) {
    FILETIME result = {};
#ifdef _WIN32
    // Wendows
    //HANDLE file_handle = CreateFileA((char*)path.str, 0x80000000, 0x00000001, NULL, 3, 0x1, NULL);
    struct FM_File file = win32_open_file(path, 0x80000000, 0x00000001, 3, 0x1);
    if (file.handle == 0) {
        // If not exists, rebuild
        DWORD err = GetLastError();
        if (err == 0x2) {return result;}
        BUILDER_LOG(BUILDER_ERROR, "Could not open %s %lu", (char*)path.str, err);
        return result;
    } 
    BUILDER_LOG(BUILDER_INFO, "Opened %s", (char*)path.str);
    BOOL success = GetFileTime(file.handle, NULL, NULL, &result);
    if (!success) {
        DWORD err = GetLastError();
        BUILDER_LOG(BUILDER_ERROR, "Could not get the file time: %lu", err);
    }
    win32_close_file(file);
#else
    // TODO Lenus get filetimes thing
#endif
    return (result);
}

function b32
bs_needs_rebuild(String8 output_path, String8 input_path) {
    b32 result = false; 
    FILETIME ft_input = bs_get_filetime_from_path(input_path);
    FILETIME ft_output = bs_get_filetime_from_path(output_path);
    // Error with the files or non existing
    // Needs to rebuild
    if (ft_input.dwHighDateTime == 0 || ft_output.dwHighDateTime == 0) {result = true;}
    // Source is newer than .exe
    if (CompareFileTime(&ft_input, &ft_output) == 1) {result = true;}
    return(result);
}

function void 
bs_cmd_append(struct BuildCmd *cmd, String8 str) {
    // TODO Create a process to append commands to the buildCmd
    struct StrNode* tmpstr = MMPushArrayZeros(&cmd->arena, struct StrNode, 1);
    tmpstr->str = str;
    sll_queue_push(cmd->list.first,cmd->list.last, tmpstr);
    cmd->count += 1;
}

function void 
bs_reset_files() {
    // TODO reset the files if something happens  
}

function String8*
bs_cmd_construct_command(struct Arena *arena, struct BuildCmd *cmd) {
    String8* result = MMPushArrayZeros(arena, String8, 1);
    result->str = MMPushArrayZeros(arena, u8, KB(4));
    u32 len = cmd->count; 
    u8* ptr = result->str;
    while (len-- > 0) {
        u64 size = cmd->list.first->str.size;
        u8* src = cmd->list.first->str.str;
        while(size-- > 0 && *src != 0) {
            *ptr++ = *src++;
            result->size += 1;
        }
        if (len > 1) {
            *ptr++ = ' ';
        }
        result->size+=1;
        sll_queue_pop(cmd->list.first, cmd->list.last);
    }
    return result;
}

function void 
bs_cmd_run(struct BuildCmd *cmd) {
    // TODO Run the commands in the buildCmd line
    if (cmd->count == 0) {
        BUILDER_LOG(BUILDER_INFO, "There is no command to run");
        return;
    }
}

int main(int argc, char **argv) {
    GO_REBUILD_YOURSELF(argc, argv);
    struct BuildCmd cmd = {};
    cmd.arena = mm_scratch_arena();
    cmd.list.arena = mm_scratch_arena();
    bs_cmd_append(&cmd, string_u8_litexpr("clang"));
    bs_cmd_append(&cmd, string_u8_litexpr("build.c"));
    bs_cmd_append(&cmd, string_u8_litexpr("-o build_all.exe"));
    {
        struct Arena scratch = mm_scratch_arena();
        String8 *command = bs_cmd_construct_command(&scratch, &cmd);
        printf("%s\n", (char*)command->str);
    }
}


//PV24013200