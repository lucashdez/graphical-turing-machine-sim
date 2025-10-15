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

/* Renderer things */
s32 pltf_renderer_begin_section(PlatformWindow *w) {
  b32 found = lwl_get_free_buffer(w->os_window);
  if (!found) {
    return -1;
  }
  WaylandState *state = w->os_window;
  ShmBuffer *buf = &state->buffers[state->cur];
  mm_memset(buf->data, 0, buf->size);
  return 0;
}

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
