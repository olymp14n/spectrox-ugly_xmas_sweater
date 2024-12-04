#include "ornaments.h"

#include <GL/gl.h>
#include <math.h>
#include "../fw/math.h"

static const fw_vec4i _flameColor1 = {255,100,0, 255};
static const fw_vec4i _flameColor2 = {255,220,0, 255};

static const fw_vec4i _starColor1 = {9, 22, 53, 255};
static const fw_vec4i _starColor2 = {255,220,0, 255};

static void renderCandle(fx_ornament *candle) {
    glColor3ub(255,255,255);
    glVertex2i(candle->x, candle->y);

    glColor3ub(candle->color.x,candle->color.y,candle->color.z);
    glVertex2i(candle->x, candle->y - 1);
}

void fx_ornaments_updateCandles(fx_ornament *candles, int length, fw_timer_data *time) {
    for (int i=0; i<length; i++) {
        const float p = .25f + .25f * sinf(time->elapsed * 16 + i) + .25f + .25f * cosf(time->elapsed * 32 + i*.5);
        candles[i].color.x = fw_math_lerp(_flameColor1.x, _flameColor2.x, p);
        candles[i].color.y = fw_math_lerp(_flameColor1.y, _flameColor2.y, p);
        candles[i].color.z = fw_math_lerp(_flameColor1.z, _flameColor2.z, p);
        candles[i].color.w = 255;
    }
}

void fx_ornaments_renderCandles(fx_ornament *candles, int length) {
    glBegin(GL_POINTS);
    for (int i=0; i<length; i++) {
        renderCandle(&candles[i]);
    }
    glEnd();
}

void fx_ornaments_renderDesireCandle(void) {
    glColor3ub(255,255,255);
    glBegin(GL_POINTS);
    glVertex2i(55,12);
    glVertex2i(54,13);
    glVertex2i(55,13);
    glVertex2i(54,14);
    glVertex2i(55,14);
    glVertex2i(54,15);
    glEnd();
}

static void renderStar(fx_ornament *star) {
    glColor4ub(star->color.x,star->color.y,star->color.z, star->color.w);
    glVertex2i(star->x, star->y);
}

void fx_ornaments_updateStars(fx_ornament *stars, int length, float p, fw_timer_data *time) {
    if (p <= 0) {
        return;
    }

    const int maxIdx = length * p;
    const float fraction = (length * p) - maxIdx;

    for (int i=0; i<length; i++) {
        const float p = .4f + .2f + .2f * sinf(time->elapsed * 4 + i*2) + .1f + .1f * cosf(time->elapsed * 24 + i*2);
        stars[i].color.x = fw_math_lerp(_starColor1.x, _starColor2.x, p);
        stars[i].color.y = fw_math_lerp(_starColor1.y, _starColor2.y, p);
        stars[i].color.z = fw_math_lerp(_starColor1.z, _starColor2.z, p);

        if (i<maxIdx) {
            stars[i].color.w = 255;
        } else if (i == maxIdx) {
            stars[i].color.w = 255 * fraction;
        } else {
            continue;
        }
    }
}

void fx_ornaments_renderStars(fx_ornament *stars, int length) {
    glBegin(GL_POINTS);
    for (int i=0; i<length; i++) {
        renderStar(&stars[i]);
    }
    glEnd();
}


