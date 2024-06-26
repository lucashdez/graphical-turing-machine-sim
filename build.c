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
    // Lenus
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
    
}

function void 
bs_reset_files() {
    
}

function void 
bs_cmd_run(struct BuildCmd *cmd) {
    
}


int main(int argc, char **argv) {
    GO_REBUILD_YOURSELF(argc, argv);
}