#include "aurora.h"
#include <math.h>
#include "../resource.h"
#include "../fw/render.h"
#include "../fw/quad.h"
#include "knit.h"

#define AURORA_MASK_DEBUG_COLORS_ENABLED 0

#define AURORA_MASK_STRIP_NUM_ROWS 3
#define AURORA_MASK_STRIP_NUM_COLS 11

static fw_vec2f _auroraMaskStripCoords[AURORA_MASK_STRIP_NUM_COLS][AURORA_MASK_STRIP_NUM_ROWS];
static fw_vec2f _groundMaskStripCoords[AURORA_MASK_STRIP_NUM_COLS][AURORA_MASK_STRIP_NUM_ROWS];

static fw_vec4i _auroraMaskStripRowColors[] = {
#if AURORA_MASK_DEBUG_COLORS_ENABLED
    {255, 0, 0, 255},
    {0, 255, 0, 255},
    {0, 0, 255, 255},
#else
    {9, 22, 53, 0},
    {9, 22, 53, 255},
    {9, 22, 53, 255},
#endif
};

static fw_vec4i _groundMaskStripRowColors[] = {
#if AURORA_MASK_DEBUG_COLORS_ENABLED
    {255, 0, 0, 255},
    {0, 255, 0, 255},
    {0, 0, 255, 255},
#else
    {3, 13, 43, 0},
    {3, 13, 43, 255},
    {3, 13, 43, 255},
#endif
};

static const int _segmentWidth = .5f + (float)FX_KNIT_CANVAS_WIDTH / (AURORA_MASK_STRIP_NUM_COLS-1);

static void initAuroraMask(void) {
    // Top mask row. Quadratic sine.
    for (int x=0; x<AURORA_MASK_STRIP_NUM_COLS; x++) {
        float yoff = 4*sinf(M_PI*x/(float)(AURORA_MASK_STRIP_NUM_COLS-1));
        yoff *= yoff;
        yoff += 8;
        yoff *= -1;
        _auroraMaskStripCoords[x][0] = (fw_vec2f) {
            x*_segmentWidth, yoff
        };
    }

    // Middle mask row. Sine.
    for (int x=0; x<AURORA_MASK_STRIP_NUM_COLS; x++) {
        float yoff = 5*sinf(M_PI*x/(float)(AURORA_MASK_STRIP_NUM_COLS-1));
        yoff += 4;
        yoff *= -1;
        _auroraMaskStripCoords[x][1] = (fw_vec2f) {
            x*_segmentWidth, yoff
        };
    }

    // Bottom mask row. Base height at 0.
    for (int x=0; x<AURORA_MASK_STRIP_NUM_COLS; x++) {
        _auroraMaskStripCoords[x][2] = (fw_vec2f) {
            x*_segmentWidth, 0
        };
    }
}

static void initGroundMask(void) {
    for (int y=0; y<AURORA_MASK_STRIP_NUM_ROWS; y++) {
        for (int x=0; x<AURORA_MASK_STRIP_NUM_COLS; x++) {
            _groundMaskStripCoords[x][y] = (fw_vec2f) {
                x*_segmentWidth,-3+y*2
            };
        }
    }
}

void fx_aurora_init(void) {
    initAuroraMask();
    initGroundMask();
}

static void renderAuroraBeams(float alpha, fw_timer_data *time) {
    glPushMatrix();
    glTranslatef(0,1,-5);
    glTranslatef(0,-1,0);
    glRotatef(85,1,0,0);
    glScalef(2.2,2.5,1);
    glTranslatef(0,1,0);

    const unsigned char finalAlpha = alpha * (80 + 30*sinf(time->elapsed*.5f));
    const float offsetX = time->elapsed * .125f;

    glColor4ub(50,180,200,finalAlpha);
    fw_quad_putTextured(8,offsetX, 1,0);
    glColor4ub(50,240,120,finalAlpha);
    fw_quad_putTextured(16,-offsetX, 1,0);
    glPopMatrix();
}

static void renderGroundBeams(float alpha, fw_timer_data *time) {
    glPushMatrix();
    glTranslatef(0,.65,-5);
    glTranslatef(0,-1,0);
    glRotatef(88,1,0,0);
    glScalef(2.2,2.0,1);
    glTranslatef(0,1,0);

    const unsigned char finalAlpha = .25f * alpha * (120 + 30*sinf(time->elapsed*.5f));
    const float offsetX = time->elapsed * .5f;

    glColor4ub(50,180,200,finalAlpha);
    fw_quad_putTextured(32,offsetX, 1,0);
    fw_quad_putTextured(16,-offsetX, 1,0);
    glPopMatrix();
}

static void renderMask(fw_vec2f coords[][AURORA_MASK_STRIP_NUM_ROWS], fw_vec4i *colors, float y, float scaleY, float alpha, fw_timer_data *time) {
    glPushMatrix();

    glTranslatef(0,y,0);
    glScalef(1,scaleY,1);
    glBegin(GL_QUADS);
    for (int y=0; y<AURORA_MASK_STRIP_NUM_ROWS-1; y++) {
        for (int x=0; x<AURORA_MASK_STRIP_NUM_COLS-1; x++) {

            glColor4ub(colors[y].x,
                       colors[y].y,
                       colors[y].z,
                       colors[y].w * alpha);
            glVertex2f(coords[x+1][y].x,coords[x+1][y].y);
            glVertex2f(coords[x][y].x,coords[x][y].y);

            glColor4ub(colors[y+1].x,
                       colors[y+1].y,
                       colors[y+1].z,
                       colors[y+1].w * alpha);
            glVertex2f(coords[x][y+1].x,coords[x][y+1].y);
            glVertex2f(coords[x+1][y+1].x,coords[x+1][y+1].y);
        }
    }
    glEnd();
    glPopMatrix();
}

void fx_aurora_render(float alpha, fw_timer_data *time) {
    fx_knit_projection_perspective();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, getImages()[RES_IMG_BEAM].textureID);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    renderAuroraBeams(alpha, time);
    renderGroundBeams(alpha, time);

    glDisable(GL_TEXTURE_2D);
    fw_render_defaultBlendMode();

    fx_knit_projection_ortho();

    // Overlay the with masks to make aurora/ground fade out nicely.
    renderMask(_auroraMaskStripCoords, _auroraMaskStripRowColors, 45, 1, 1, time);
    renderMask(_groundMaskStripCoords, _groundMaskStripRowColors, 53, -1, alpha, time);
}
