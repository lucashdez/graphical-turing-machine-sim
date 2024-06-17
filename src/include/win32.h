/* date = June 13th 2024 2:49 pm */

#ifndef WIN32_H
#define WIN32_H


//~ NOTE(lucashdez): Types
# define APIENTRY __stdcall

# define CALLBACK __stdcall
typedef void* HINSTANCE;

typedef char* LPSTR;

# define WINAPI __stdcall 

//~ NOTE(lucashdez): extern
extern int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);

#endif //WIN32_H
