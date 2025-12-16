#ifndef PLATFORM_H
#  define PLATFORM_H

#  ifdef _WIN32
#    include "win32/win32.h" 
#  endif

#include "vulkan/vulkan.h"

#include "../base/base.h"


/* ~ ENUMS */

typedef enum _PlatformEvent 
{
    PltfNone,
	PltfResize,
} PlatformEvent;

typedef enum _PlatformPresentKind
{
    PlatformPresent_None,
    PlatformPresent_Software,
    PlatformPresent_Vulkan,
} PlatformPresentKind;

/*~ STRUCTS */

typedef struct PlatformFrame {
    u64 dt;
} PlatformFrame;

typedef struct PlatformPresent {
    PlatformPresentKind kind;
    u32 width;
    u32 height;

    union {
        struct {
            void *pixels; /* u32* */
            u32 pitch;
        } sw;

        struct {
            void *image; /* VkImage */
            void *queue; /* VkQueue */
        } vk;
    };
} PlatformPresent;

typedef struct PlatformWindow {
    s32 width;
    s32 height;
	PlatformFrame frame_info;
	void* os_window;
} PlatformWindow;



/* ~ API */
internal void* pltf_mem_reserve();
internal void* pltf_mem_commit();
internal void pltf_mem_decommit(void* ptr, u64 size);
internal void pltf_mem_release(void*);
internal void pltf_print(const char* s);

/* Window things */
extern PlatformWindow* pltf_window_create(Arena *arena, s32 width, s32 height, struct StringConstU8 title);
extern void pltf_window_destroy(PlatformWindow* win);
extern Vec2 pltf_get_pointer_pos(PlatformWindow* win);

/* Frame things */
PlatformPresent pltf_begin_present(PlatformWindow *win);
void pltf_end_present(PlatformWindow *win);
extern void pltf_poll_events(PlatformWindow* win);
extern u64 pltf_timestamp(void);
typedef void (*PlatformFrameCallback)(PlatformWindow *wnd, void *user_pointer);


/* main loop */ 
extern void pltf_window_set_frame_callback(PlatformWindow *wnd, PlatformFrameCallback cb, void* user);
extern void pltf_window_run_loop(PlatformWindow *wnd);

#endif /* PLATFORM_H */
