#include "build.h"


function b32
needs_rebuild(String8 output_path, String8 input_path) {
    b32 result = true; 
#ifdef _WIN32
    // Wendows
    HANDLE file_handle = CreateFileA((char*)output_path.str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    if (!file_handle) {
        // If not exists, rebuild
        if (GetLastError() == WIN32_ERROR_FILE_NOT_FOUND) {return 1;}
        BUILDER_LOG(BUILDER_ERROR, "Could not open %s", (char*)output_path.str);
        return -1;
    } 
    BUILDER_LOG(BUILDER_INFO, "Opened %s", (char*)output_path.str);
    FILETIME output_file_time;
    BOOL success = GetFileTime(file_handle, NULL, NULL, &output_file_time);
    if (!success) {
        DWORD err = GetLastError();
        
        BUILDER_LOG(BUILDER_ERROR, "Could not get the file time: %lu", err);
        return -1;
    }
    CloseHandle(file_handle);
#else
    // Lenus
#endif
    return(result);
}

function void 
cmd_append(struct BuildCmd *cmd, String8 str) {
    
}

function void 
reset_files() {
    
}

function void 
cmd_run(struct BuildCmd *cmd) {
    
}


int main(int argc, char **argv) {
    GO_REBUILD_YOURSELF(argc, argv);
}