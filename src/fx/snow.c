#include "snow.h"
#include "knit.h"
#include <math.h>
#include "../fw/math.h"

void fx_snow_init(fx_snow_flake *flakes, int numFlakes, int smallFlakePercentage) {
    srand(1338);
    for (int i=0; i<numFlakes; i++) {
        flakes[i].x = fw_math_rand(0, FX_KNIT_CANVAS_WIDTH-1);
        flakes[i].y = -fw_math_rand(3, FX_KNIT_CANVAS_HEIGHT);
        flakes[i].size = fw_math_rand(0, 100) > smallFlakePercentage ? 2 : 1;

        if (flakes[i].size == 1) {
            flakes[i].speed = fw_math_rand(5,20) * 1.5f;
        } else {
            flakes[i].speed = fw_math_rand(16,24) * 1.5f;
        }
    }
}

void fx_snow_update(fx_snow_flake *flakes, int numFlakes, int isRespawnEnabled, fw_timer_data *time) {
    for (int i=0; i<numFlakes; i++) {
        flakes[i].y += time->delta * flakes[i].speed;
        flakes[i].xOff = 2.5 * sinf(i*.2 + time->elapsed * flakes[i].size * 2);
        if (isRespawnEnabled && flakes[i].y > FX_KNIT_CANVAS_HEIGHT) {
            flakes[i].x = fw_math_rand(0, FX_KNIT_CANVAS_WIDTH-1);
            flakes[i].y = -3;
        }
    }
}

void fx_snow_render(fx_snow_flake *flakes, int numFlakes, int r, int g, int b) {
    glBegin(GL_POINTS);
    glColor3ub(r,g,b);

    for (int i=0; i<numFlakes; i++) {
        if (flakes[i].size == 1) {
            glVertex2f(flakes[i].x + flakes[i].xOff, flakes[i].y);
        } else {
            glVertex2f(flakes[i].x + flakes[i].xOff, flakes[i].y);
            //glVertex2f(flakes[i].x + flakes[i].xOff, 1+flakes[i].y);
            glVertex2f(1+flakes[i].x + flakes[i].xOff, 1+flakes[i].y);
            glVertex2f(flakes[i].x + flakes[i].xOff, 2+flakes[i].y);
            glVertex2f(-1+flakes[i].x + flakes[i].xOff, 1+flakes[i].y);
        }
    }
    glEnd();
}
