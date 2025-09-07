/* date = September 6th 2025 10:15 pm */

#ifndef RENDERER_H
#define RENDERER_H

typedef struct RendererState 
{
    u32 color; //AARRGGBB
} RendererState;



internal void renderer_init(PlatformWindow* window);
internal void renderer_shutdown(void);

internal void renderer_clear(u32 color);
internal void renderer_present(void);

// Primitivas
internal void renderer_draw_pixel(s32 x, s32 y, u32 color);
internal void renderer_draw_line(s32 x0, s32 y0, s32 x1, s32 y1, u32 color);
internal void renderer_draw_rect(s32 x, s32 y, s32 w, s32 h, u32 color, b32 filled);
internal void renderer_draw_triangle(Vec2 a, Vec2 b, Vec2 c, u32 color, b32 filled);
internal void renderer_draw_circle(s32 cx, s32 cy, s32 radius, u32 color, b32 filled);



#endif //RENDERER_H
