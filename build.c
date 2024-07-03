#include "build.h"
#include <stdlib.h>

function i32
bs_run_command(String8* command) {
    i32 result = true;
    if (command->size == 0) {
        BUILDER_LOG(BUILDER_ERROR, "Cannot run a void command.");
        return false;
    }
#ifdef _WIN32
    STARTUPINFOA siStartInfo = {0};
    siStartInfo.hStdInput = stdin;
    siStartInfo.hStdOutput = stdout;
    siStartInfo.hStdError = stderr;
    PROCESS_INFORMATION piProcInfo = {0};
    BOOL bSuccess = CreateProcessA(0, (char*)command->str, 0, 0, true, 0, 0, 0, &siStartInfo, &piProcInfo);
    if (!bSuccess) {
        BUILDER_LOG_ARGS(BUILDER_ERROR, "Cannot create a child process: %lu", GetLastError());
        result = false;
        //a
    }
#else
    // TODO LINUX FORK
#endif
    return(result);
}

function void 
bs_get_builder_command(struct BuildCmd *cmd) {
    bs_cmd_append(cmd, string_u8_litexpr("clang -g"));
#ifdef _WIN32
    bs_cmd_append(cmd, string_u8_litexpr("-gcodeview"));
#endif
    bs_cmd_append(cmd, string_u8_litexpr("-Isrc/include build.c -o"));
#ifdef _WIN32
    bs_cmd_append(cmd, string_u8_litexpr("build_all.exe"));
#else
    bs_cmd_append(cmd, string_u8_litexpr("build_all"));
#endif
}

function i32 bs_compare_file_time(struct FM_File* a, struct FM_File* b) {
    return a->info.mod_time < b->info.mod_time? -1 : a->info.mod_time == b->info.mod_time? 0 : 1;
}

function i32
bs_rename_file(String8 old_path, String8 new_path) {
    BUILDER_LOG_ARGS(BUILDER_INFO, "Renaming %s -> %s", old_path.str, new_path.str);
#ifdef _WIN32
    if (!MoveFileExA((char*)old_path.str, (char*)new_path.str, 0x1)) {
        BUILDER_LOG_ARGS(BUILDER_ERROR, "Error renaming file from %s -> %s: %lu", old_path.str, new_path.str, GetLastError());
        return false;
    }
#else
    //linux
    //if (!rename(old_path.str, new_path.str)) {
    //
    //}
#endif
    return true;
}

internal struct FM_File
bs_get_file_from_path(String8 path) {
    struct FM_File result = {};
#ifdef _WIN32
    // Wendows
    //HANDLE file_handle = CreateFileA((char*)path.str, 0x80000000, 0x00000001, NULL, 3, 0x1, NULL);
    struct FM_File file = win32_open_file(path, 0x80000000, 0x00000001, 3, 0x1);
    if (file.handle == 0) {
        // If not exists, rebuild
        DWORD err = GetLastError();
        if (err == 0x2) {return result;}
        BUILDER_LOG_ARGS(BUILDER_ERROR, "Could not open %s %lu", (char*)path.str, err);
        return result;
    }
    BUILDER_LOG_ARGS(BUILDER_INFO, "Opened %s", (char*)path.str);
    {
        FILETIME tmp_filetime = {};
        BOOL success = GetFileTime(file.handle, NULL, NULL, &tmp_filetime);
        if (!success) {
            DWORD err = GetLastError();
            BUILDER_LOG_ARGS(BUILDER_ERROR, "Could not get the file time: %lu", err);
            return result;
        }
        // NOTE Something we must do with the | operator? 
        result.info.mod_time = tmp_filetime.dwHighDateTime | tmp_filetime.dwLowDateTime;
    }
    win32_close_file(file);
#else
    // TODO Lenus get filetimes thing
    struct Stat buf = {};
    stat((char*)path.str, &buf);
    printf("%lld", buf.st_mtime);
#endif
    return (result);
}

function b32
bs_needs_rebuild(String8 output_path, String8 input_path) {
    b32 result = false; 
    struct FM_File ft_input = bs_get_file_from_path(input_path);
    struct FM_File ft_output = bs_get_file_from_path(output_path);
    // Error with the files or non existing
    // Needs to rebuild
    if (ft_input.info.mod_time == 0 || ft_output.info.mod_time == 0) {result = true;}
    // Source is newer than .exe
    if (bs_compare_file_time(&ft_input, &ft_output) == 1) {result = true;}
    return(result);
}

function void 
bs_cmd_append(struct BuildCmd *cmd, String8 str) {
    struct StrNode* tmpstr = MMPushArrayZeros(&cmd->arena, struct StrNode, 1);
    tmpstr->str = str;
    sll_queue_push(cmd->list.first,cmd->list.last, tmpstr);
    cmd->count += 1;
}

function void 
bs_reset_files() {
#ifdef _WIN32
	if(!MoveFileExA("build.old", "build_all.exe", 0x1)) {
		BUILDER_LOG_ARGS(BUILDER_ERROR, "Could not reset files: %lu", GetLastError());
	}
#else
    
#endif
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
    GLOBAL_BASE_ALLOCATOR = mm_create_malloc_base_memory();
    GO_REBUILD_YOURSELF(argc, argv);
    struct BuildCmd cmd = {};
    cmd.arena = mm_scratch_arena();
    cmd.list.arena = mm_scratch_arena();
    bs_cmd_append(&cmd, string_u8_litexpr("clang -g"));
#ifdef _WIN32
    bs_cmd_append(&cmd, string_u8_litexpr("-gcodeview"));
#endif
    bs_cmd_append(&cmd, string_u8_litexpr("-Isrc/include -Isrc/impls"));
    bs_cmd_append(&cmd, string_u8_litexpr("src/main.c"));
    bs_cmd_append(&cmd, string_u8_litexpr("-o "));
#ifdef _WIN32
    bs_cmd_append(&cmd, string_u8_litexpr("build/gtms.exe"));
#else
    bs_cmd_append(&cmd, string_u8_litexpr("build/gtms"));
#endif
    {
        struct Arena scratch = mm_scratch_arena();
        String8 *command = bs_cmd_construct_command(&scratch, &cmd);
        bs_run_command(command);
    }
    
}


//PV24013200
