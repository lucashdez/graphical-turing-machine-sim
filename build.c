#include "build.h"
#include <stdlib.h>

internal FILETIME
bs_get_filetime_from_path(String8 path) {
    FILETIME result = {};
#ifdef _WIN32
    // Wendows
    HANDLE file_handle = CreateFileA((char*)path.str, 0x80000000, 0x00000001, NULL, 3, 0x1, NULL);
    if (file_handle == 0) {
        // If not exists, rebuild
        DWORD err = GetLastError();
        if (err == 0x2) {return result;}
        BUILDER_LOG(BUILDER_ERROR, "Could not open %s %lu", (char*)path.str, err);
        return result;
    } 
    BUILDER_LOG(BUILDER_INFO, "Opened %s", (char*)path.str);
    BOOL success = GetFileTime(file_handle, NULL, NULL, &result);
    if (!success) {
        DWORD err = GetLastError();
        BUILDER_LOG(BUILDER_ERROR, "Could not get the file time: %lu", err);
        return (FILETIME) {};
    }
    CloseHandle(file_handle);
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
bs_cmd_append_s8(struct BuildCmd *cmd, String8 str) {
    // TODO Create a process to append commands to the buildCmd
}

function void 
bs_cmd_append(struct BuildCmd *cmd, char* str) {
    // TODO Create a process to append commands to the buildCmd
}


function void 
bs_reset_files() {
    // TODO reset the files if something happens  
}

function String8*
bs_cmd_construct_command(struct Arena *arena, struct BuildCmd *cmd) {
    String8* result = MMPushArrayZeros(arena, String8, 1);
    u32 len = cmd->count; 
    printf("%d\n", len);
    u32 i = 0; 
    u8* res_ptr = result->str; 
    while (len-- > 0) {
        result->str = cmd->items[i].str;
        mm_memcpy(res_ptr, cmd->items[i].str, cmd->items[i].size);
        result->size += cmd->items[i].size;
        i++;
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


struct node {
    struct node* next;
    i32 value;
};

int main(int argc, char **argv) {
    GO_REBUILD_YOURSELF(argc, argv);
    struct BuildCmd cmd = {};
    cmd.arena = mm_scratch_arena();
    bs_cmd_append(&cmd, "clang");
    {
        struct Arena scratch = mm_scratch_arena();
        String8 *command = bs_cmd_construct_command(&scratch, &cmd);
        printf("%s\n", (char*)command->str);
    }
}