#include "../../renderer.h"

internal s32 renderer_begin_section(PlatformWindow *w) {
  if (pltf_renderer_begin_section(w) < 0)
	return -1;
}

internal void renderer_end_section(PlatformWindow *w) {
  
	return;
}

static s32 xOffset = 0;
internal void
renderer_draw_rect(PlatformWindow *window, s32 x, s32 y, s32 w, s32 h, u32 color, b32 filled)
{
	u32* fb = pltf_get_framebuffer(window);

	x = x + xOffset;
	
	/* s32 sx1 = (x * window->width) / VIRTUAL_WIDTH; */
	/* s32 sy1 = (y * window->height) / VIRTUAL_HEIGTH; */
	/* s32 sx2 = ((x+w) * window->width) / VIRTUAL_WIDTH; */
	/* s32 sy2 = ((y+h) * window->height) / VIRTUAL_HEIGTH; */


	s32 y1, x1;
	for(y1 = y; y1 < y + h; ++y1)
	  for(x1 = x; x1 < x + w; ++x1) {
		fb[y1 * 1920 + x1] = color;

	  }
	


	/* for(s32 y2 = sy1; y2 < sy2; ++y2) */
	/* for (s32 x2 = sx1 ; x2 < sx2; ++x2) */
	/*   { */
	/* 	fb[y2*window->width+x2] = color; */
	/*   } */

	xOffset = (xOffset + 1) % 100;
}
