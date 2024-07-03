/**
 * This is the win32 main file for the gtms testing program.
 * The opening of windows and the main loop is represented here.
 * 
 */
#ifdef _WIN32

static struct FM_File 
win32_open_file(String8 path, u32 access, u32 mode, u32 options, u32 flags) {
    struct FM_File file = {0};
    file.handle = CreateFileA((char*)path.str, access, mode, 0, options, flags, 0);
    file.path = path;
    FILETIME ft = {0};  
    BOOL success = GetFileTime(file.handle, NULL, NULL, &ft);   
    file.info.mod_time =  ft.dwHighDateTime;
    return (file);
}

static void 
win32_close_file(struct FM_File file) {
    CloseHandle(file.handle);
}

int WINAPI 
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    GLOBAL_BASE_ALLOCATOR = mm_create_malloc_base_memory();
    struct Arena arena = mm_scratch_arena();
    String8* s = MMPushArrayZeros(&arena, String8, 1);
    s->str = (u8*)"a";
    printf("%s\n", (char*)s->str);
}
#endif
