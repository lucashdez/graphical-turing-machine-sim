#include "../../renderer.h"

internal s32 renderer_begin_section(PlatformWindow *w) {
  if (pltf_renderer_begin_section(w) < 0)
	return -1;
}

internal void renderer_end_section(PlatformWindow *w) {
  
	return;
}

internal void
renderer_draw_rect(PlatformWindow *window, Rects32 rect, u32 color, b32 filled)
{
	u32* fb = pltf_get_framebuffer(window);
	s32 rx1 = (rect.p.x * window->width) / VIRTUAL_WIDTH;
	s32 rx2 = ((rect.p.x + rect.width) * window->width) / VIRTUAL_WIDTH;
	s32 ry1 = (rect.p.y * window->height) / VIRTUAL_HEIGHT;
	s32 ry2 = ((rect.p.y + rect.height) * window->height) / VIRTUAL_HEIGHT;
	s32 y1, x1;
	for (y1 = ry1; y1 < ry2; ++y1)
		for (x1 = rx1; x1 < rx2; ++x1)
			{
				fb[y1 * window->width + x1] = color;
			}
}
