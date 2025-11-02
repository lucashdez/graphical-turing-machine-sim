#include "../../renderer.h"
#include <math.h>

internal s32 renderer_begin_section(PlatformWindow *w) {
  if (pltf_renderer_begin_section(w) < 0)
	return -1;
}

internal void renderer_end_section(PlatformWindow *w) {
  
	return;
}

internal void
renderer_draw_rect(PlatformWindow *window, Rects32 rect, u32 color, b32 filled, s32 thickness)
{
	u32* fb = pltf_get_framebuffer(window);
	s32 rx1 = (rect.p.x * window->width) / VIRTUAL_WIDTH;
	s32 rx2 = ((rect.p.x + rect.width) * window->width) / VIRTUAL_WIDTH;
	s32 ry1 = (rect.p.y * window->height) / VIRTUAL_HEIGHT;
	s32 ry2 = ((rect.p.y + rect.height) * window->height) / VIRTUAL_HEIGHT;
	if (filled)
	{
		s32 y1, x1;
		for (y1 = ry1; y1 < ry2; ++y1)
			for (x1 = rx1; x1 < rx2; ++x1)
				{
					fb[y1 * window->width + x1] = color;
				}
	}
	else
	{
		for (s32 y = ry1; y < ry1 + thickness && y <= ry2; ++y)
			for (s32 x = rx1; x <= rx2; ++x)
				fb[y * window->width + x] = color;

		for (s32 y = ry2 - thickness + 1; y <= ry2; ++y)
			for (s32 x = rx1; x <= rx2; ++x)
				fb[y * window->width + x] = color;

		for (s32 y = ry1 + thickness; y <= ry2 - thickness; ++y)
			for (s32 x = rx1; x < rx1 + thickness && x <= rx2; ++x)
				fb[y * window->width + x] = color;

		for (s32 y = ry1 + thickness; y <= ry2 - thickness; ++y)
			for (s32 x = rx2 - thickness + 1; x <= rx2; ++x)
				fb[y * window->width + x] = color;
	}
}

 

internal void renderer_draw_circle(PlatformWindow *window, s32 cx, s32 cy, s32 radius, u32 color, b32 filled) {
  static const f32 PI = 3.1415926335;
  u32* fb = pltf_get_framebuffer(window);
  f32 i, angle, x1, y1;
  s32 cx1 = (cx * window->width) / VIRTUAL_WIDTH;
  s32 cy1 = (cy * window->height) / VIRTUAL_HEIGHT;
  
  for (i = 0; i < 360; i+=0.1)
    {
      angle = i;
      x1 = (f32)radius * cos(angle * PI / 180);
      y1 = (f32)radius * sin(angle * PI / 180);
      fb[((cy1+(s32)y1) * window->width) + (cx1+(s32)x1)] = color;
    }
}

internal void
renderer_draw_pixel(PlatformWindow *window, s32 x, s32 y, u32 color)
{
 u32 *fb = pltf_get_framebuffer(window);
 s32 nx = (x * window->width) / VIRTUAL_WIDTH;
 s32 ny = (y * window->height) / VIRTUAL_HEIGHT;
 fb[ny*window->width+nx] = color;
}    
