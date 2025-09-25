#ifndef PLATFORM_H
#  define PLATFORM_H

#  ifdef _WIN32
#    include "win32/win32.h" 
#  endif

#include "../base/base.h"

/*~ STRUCTS */

typedef struct PlatformFrame {
    u64 dt;
    u32 width, height;
} PlatformFrame;

typedef struct PlatformWindow {
    s32 width;
    s32 height;
	PlatformFrame frame_info;
	void* os_window;
} PlatformWindow;


/* ~ ENUMS */

typedef enum PlatformEvent {
    PltfNone,
} PlatformEvent;

/* ~ API */
internal void* pltf_mem_reserve();
internal void* pltf_mem_commit();
internal void pltf_mem_decommit(void* ptr, u64 size);
internal void pltf_mem_release(void*);
internal void pltf_print(const char* s);

/* Window things */
extern PlatformWindow* pltf_window_create(Arena *arena, s32 width, s32 height, struct StringConstU8 title);
extern void pltf_window_destroy(PlatformWindow* win);

/* Frame things */
extern void pltf_window_present_frame(PlatformWindow* win, void* pixels, u32 pitch);
extern void pltf_poll_events(PlatformWindow* win);
extern u64 pltf_timestamp(void);
typedef void (*PlatformFrameCallback)(PlatformWindow *wnd, void *user_pointer);

/* main loop */ 
extern void pltf_window_set_frame_callback(PlatformWindow *wnd, PlatformFrameCallback cb, void* user);
extern void pltf_window_run_loop(PlatformWindow *wnd);


/* Drawing related functions */
extern void* pltf_get_framebuffer(PlatformWindow *wnd);
extern s32 pltf_renderer_begin_section(PlatformWindow *w);

#endif /* PLATFORM_H */
