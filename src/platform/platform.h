#ifndef PLATFORM_H
#  define PLATFORM_H

#  ifdef _WIN32
#    include "win32/win32.h" 
#  endif

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"




//~ STRUCTS


typedef struct PlatformWindow {
    s32 width;
    s32 height;
    void* os_window; // This is a pointer to a opaque struct
} PlatformWindow;

//~ ENUMS

typedef enum PlatformEvent {
    PltfNone,
    
} PlatformEvent;




//~ MEMORY
void* pltf_mem_reserve();
void* pltf_mem_commit();
void pltf_mem_decommit(void* ptr, u64 size);
void pltf_mem_release(void*);


//~ DEBUGGING
void pltf_print(const char* s);




#endif //PLATFORM_H