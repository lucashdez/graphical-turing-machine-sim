#include "wayland.h"

internal b32 lwl_get_free_buffer(WaylandState *state) {
    int i;
    for (i = 0; i < 3; i++) {
        if (!state->buffers[i].busy) {
            state->cur = i;
            return 1;
        }
    }
    return 0;
}

/**********************/
/* API IMPLEMENTATION */
/**********************/

extern void *pltf_get_framebuffer(PlatformWindow *wnd) {
    WaylandState *s = wnd->os_window;
    ShmBuffer *buf = &s->buffers[s->cur];
    if (!buf) {
        return 0;
    }
    buf->busy = 1;
    return buf->data;
}

/* Window things */
extern Vec2
pltf_get_pointer_pos(PlatformWindow *win)
{
    WaylandState *s = win->os_window;
    Vec2 pos = {
        .x = (((f32)s->pointer_x)/((f32)win->width)) * VIRTUAL_WIDTH ,
        .y =  (((f32)s->pointer_y)/((f32)win->height)) * VIRTUAL_HEIGHT
    };
    return pos;
}

extern void pltf_window_set_frame_callback(PlatformWindow *wnd, PlatformFrameCallback cb, void* user)
{
    WaylandState* wl_window = wnd->os_window;
    wl_window->frame_cb = cb;
    wl_window->frame_user = user;
}


PlatformPresent pltf_begin_present(PlatformWindow *win)
{
    WaylandState *s = win->os_window;
    ASSERT(!s->present_active);
    while(!lwl_get_free_buffer(s)) {
        wl_display_dispatch(s->display);
    }

    ShmBuffer *buf = &s->buffers[s->cur];
    buf->busy = 1;

    s->present_active = 1;
    PlatformPresent p = {0};
    p.kind = PlatformPresent_Software;
    p.width = win->width;
    p.height = win->height;
    p.sw.pixels = buf->data;
    p.sw.pitch = buf->stride;
    return p;
}

void pltf_end_present(PlatformWindow *win)
{
    WaylandState *s = win->os_window;
    ASSERT(s->present_active);
    ShmBuffer *buf = &s->buffers[s->cur];
    wl_surface_attach(s->surface, buf->buffer, 0, 0);
    wl_surface_damage_buffer(s->surface, 0, 0, s->width, s->height);
    wl_surface_commit(s->surface);
    s->cur = (s->cur + 1) % 3;
    s->present_active = false;
}
