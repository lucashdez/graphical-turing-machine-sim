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
typedef void* LPCVOID;
typedef void* PCVOID;

# define WINAPI __stdcall 

//~ NOTE(lucashdez): extern


//  Enums
enum FileGenericAccessRights {
    GENERIC_ALL     = 0x10000000,
    GENERIC_EXECUTE = 0x20000000,
    GENERIC_READ    = 0x40000000,
    GENERIC_WRITE   = 0x80000000,
};

enum FileCreationDisposition {
    CREATE_NEW,
    CREATE_ALWAYS,
    OPEN_EXISTING,
    OPEN_ALWAYS,
    TRUNCATE_EXISTING,
};

enum FileAttributes {
    FILE_ATTRIBUTE_READONLY= 0x1,
    FILE_ATTRIBUTE_HIDDEN  = 0x2,
    FILE_ATTRIBUTE_SYSTEM  = 0x4,
    FILE_ATTRIBUTE_ARCHIVE = 0x20,
    FILE_ATTRIBUTE_NORMAL  = 0x80,
    FILE_ATTRIBUTE_TEMPORARY=0x100,
    FILE_ATTRIBUTE_OFFLINE = 0x1000,
    FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 0x2000,
};

enum Win32ErrorList {
    WIN32_ERROR_SUCCESS = 0x0,
    WIN32_ERROR_FILE_NOT_FOUND = 0x2,
};

//  Structs

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

//  Functions
extern void CloseHandle(HANDLE handle);
extern HANDLE CreateFileA(LPCSTR lpFileHandle, DWORD dwDesiredAccess, DWORD dwSharedMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE htemplateFile);
extern DWORD FormatMessageA(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPSTR lpBuffer, DWORD nSize, va_list *Arguments);
extern BOOL GetFileTime(HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime);
extern DWORD GetLastError();
extern int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);

#endif //WIN32_H
#endif