void platform_print(const char* s) {
#ifdef _WIN32
    OutputDebugString(s);
#endif
}