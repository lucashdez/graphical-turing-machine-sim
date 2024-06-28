#ifdef _WIN32

function struct FM_File 
win32_open_file(String8 path, u32 access, u32 mode, u32 options, u32 flags) {
    struct FM_File file = {0};
    file.handle = CreateFileA((char*)path.str, access, mode, 0, options, flags, 0);
    file.path = path;
    FILETIME ft = {0};  
    BOOL success = GetFileTime(file.handle, NULL, NULL, &ft);   
    file.info.mod_time =  ft.dwHighDateTime;
    return (file);
}

function void 
win32_close_file(struct FM_File file) {
    CloseHandle(file.handle);
}

int WINAPI 
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    printf("Hello, windows!!\n");
}
#endif