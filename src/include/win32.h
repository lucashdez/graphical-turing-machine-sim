#ifdef _WIN32
#ifndef WIN32_H
#define WIN32_H


//~ NOTE(lucashdez): Types
# define APIENTRY __stdcall

# define CALLBACK __stdcall
typedef int BOOL;
typedef unsigned long DWORD;
typedef void* HANDLE;
typedef void* HINSTANCE;

typedef const char* LPCSTR;
#ifdef _WIN64
typedef u32* LPTSTR;
#else
typedef const char* LPTSTR;
#endif
typedef char* LPSTR;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef const void* PCVOID;

# define WINAPI __stdcall 

//~ NOTE(lucashdez): extern

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

// INTERN FUNCTIONS
function struct FM_File win32_open_file(String8 path, u32 access, u32 mode, u32 options, u32 flags);
function void win32_close_file(struct FM_File file);

// Extern Functions
extern void CloseHandle(HANDLE handle);
extern long CompareFileTime(const FILETIME* lpFileTime1, const FILETIME* lpFileTime2);
extern HANDLE CreateFileA(LPCSTR lpFileHandle, DWORD dwDesiredAccess, DWORD dwSharedMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE htemplateFile);
extern DWORD FormatMessageA(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPSTR lpBuffer, DWORD nSize, va_list *Arguments);
extern BOOL GetFileTime(HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime);
extern DWORD GetLastError();
extern BOOL MoveFileExA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, DWORD dwFlags);
extern int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);

#endif //WIN32_H
#endif