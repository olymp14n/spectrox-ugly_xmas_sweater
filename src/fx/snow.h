#ifndef FX_SNOW_H
#define FX_SNOW_H

#include "../fw/types.h"

typedef struct {
    float x;
    float xOff;
    float y;
    float speed;
    int size;
} fx_snow_flake;

void fx_snow_init(fx_snow_flake *flakes, int numFlakes, int smallFlakePercentage);
void fx_snow_update(fx_snow_flake *flakes, int numFlakes, int isRespawnEnabled, fw_timer_data *time);
void fx_snow_render(fx_snow_flake *flakes, int numFlakes, int r, int g, int b);

#endif
