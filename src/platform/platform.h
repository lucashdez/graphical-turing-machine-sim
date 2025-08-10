#ifndef PLATFORM_H
#  define PLATFORM_H

#  ifdef _WIN32
#    include "win32/win32.h" 
#  endif

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"


/* 
*  ############
*    STRUCTS
  *  ############
*/

typedef struct PlatformWindow {
    s32 width;
    s32 height;
    GLFWwindow *pwindow;
} PlatformWindow;

/* 
*  ############
 *      API
  *  ############
*/ 
// MEMORY
void* platform_mem_reserve();
void* platform_mem_commit();
void platform_mem_decommit(void* ptr, u64 size);
void platform_mem_release(void*);


// DEBUG
void platform_print(const char* s);

#endif //PLATFORM_H