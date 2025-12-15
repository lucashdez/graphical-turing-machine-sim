/* date = September 6th 2025 10:15 pm */

#ifndef RENDERER_H
#define RENDERER_H

#include "../platform/platform.h"

#define ION_ASSERT(x) ASSERT(x)

/* Startup */
void ion_init(PlatformWindow* window);
void ion_shutdown(void);

/* frame */
void ion_begin_frame(void);
void ion_clear(u32 color);
void ion_end_frame(void);
void ion_present(void);

/*  Primitivas */
void ion_draw_pixel(s32 x, s32 y, u32 color);
void ion_draw_line(s32 x0, s32 y0, s32 x1, s32 y1, u32 color);
void ion_draw_rect(Rects32 rect, u32 color, b32 filled, s32 thickness);
void ion_draw_circle(s32 cx, s32 cy, s32 radius, u32 color, b32 filled);

#endif /* RENDERER_H */
