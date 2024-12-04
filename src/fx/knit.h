#ifndef FX_KNIT_H
#define FX_KNIT_H

#include "../fw/image.h"

#define FX_KNIT_CANVAS_WIDTH 99
#define FX_KNIT_CANVAS_HEIGHT 90

void fx_knit_init(void);

void fx_knit_viewport(int xOffset, int yOffset) ;
void fx_knit_readPixels(void);
void fx_knit_projection_ortho(void);
void fx_knit_projection_perspective(void);
void fx_knit_render(fw_timer_data *time);
void fx_knit_imageBegin(fw_image *image);
void fx_knit_imageEnd(void);

#endif
