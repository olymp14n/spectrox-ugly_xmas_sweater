#ifndef FX_ORNAMENTS_H
#define FX_ORNAMENTS_H

#include "../fw/types.h"

typedef struct {
    int x;
    int y;
    fw_vec4i color;
} fx_ornament;

void fx_ornaments_updateCandles(fx_ornament *candles, int length, fw_timer_data *time);
void fx_ornaments_renderCandles(fx_ornament *candles, int length);

void fx_ornaments_renderDesireCandle(void);

void fx_ornaments_updateStars(fx_ornament *stars, int length, float p, fw_timer_data *time);
void fx_ornaments_renderStars(fx_ornament *stars, int length);

#endif
