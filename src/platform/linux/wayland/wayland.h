#ifndef WAYLAND_H
#  define WAYLAND_H

#include "../../platform.h"


/* Structs */


typedef struct ShmBuffer {
    struct wl_buffer *buffer;
    void *data;
    s32 size;
    s32 stride;
    b32 busy;
} ShmBuffer;

typedef struct WaylandState {
    struct wl_display *display;
    struct wl_compositor *compositor;
    struct wl_shm *shm;
    
    struct wl_surface *surface;
    /*  xdg */
    struct xdg_wm_base *x_wm_base;
    struct xdg_surface *x_surface;
    struct xdg_toplevel *x_toplevel;
    
    /*  INPUT: seat */
    struct wl_seat *seat;
    struct wl_pointer *pointer;
    
    u32 *framebuffer;
    PlatformFrameCallback frame_cb;
    void *frame_user;
    
    u32 last_frame;
    
    s32 width;
    s32 height;
    
    /*  buffer thingy */
    struct wl_buffer *buffer;
    void* data_buffer;
    
    ShmBuffer buffers[3];
    s32 cur;
    
    b32 running;
    b32 configured;
    
    s32 pointer_x;
    s32 pointer_y;
    
} WaylandState;

global_var WaylandState *wl_state = 0;

/* Functions */

internal b32 lwl_get_free_buffer(WaylandState *state);


#endif
