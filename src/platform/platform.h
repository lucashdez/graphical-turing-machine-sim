#ifndef PLATFORM_H
#  define PLATFORM_H

#  ifdef _WIN32
#    include "win32/win32.h" 
#  endif

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "../base/base.h"



//~ STRUCTS

typedef struct PlatformFrame {
    u64 dt;
    u32 width, height;
} PlatformFrame;

typedef struct PlatformWindow {
    s32 width;
    s32 height;
    void* os_window; // This is a pointer to a opaque struct
} PlatformWindow;


//~ ENUMS

typedef enum PlatformEvent {
    PltfNone,
    
    
} PlatformEvent;




//~ API
internal void* pltf_mem_reserve();
internal void* pltf_mem_commit();
internal void pltf_mem_decommit(void* ptr, u64 size);
internal void pltf_mem_release(void*);
internal void pltf_print(const char* s);

internal PlatformWindow* pltf_window_create(Arena *arena, s32 width, s32 height, struct StringConstU8 title);
internal void pltf_window_destroy(PlatformWindow* win);

internal void pltf_window_present_frame(PlatformWindow* win, void* pixels, u32 pitch);
internal void pltf_poll_events(PlatformWindow* win);
internal u64 pltf_timestamp(void);
typedef void (*PlatformFrameCallback)(PlatformFrame *frame, void *user_pointer);

/** main loop */ 
internal void pltf_window_set_frame_callback(PlatformWindow *wnd, PlatformFrameCallback cb, void* user);
internal void pltf_window_run_loop(PlatformWindow *wnd);

#endif //PLATFORM_H
