#include "twister.h"
#include <GL/glu.h>
#include <math.h>
#include "../resource.h"
#include "../fw/quad.h"
#include "../fw/stencil.h"
#include "knit.h"

static fw_vec3i _colors[] = {
    {38,140,50},
    {32,119,42},
    {26,99,35},
    {21,79,28},
};

static void renderTwisterCube(void) {
    glScalef(6,.25,.5);

    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    {
        glColor3ub(_colors[0].x,_colors[0].y,_colors[0].z);
        glTranslatef(0,0,1);
        fw_quad_put(-1,-1,2,2);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3ub(_colors[1].x,_colors[1].y,_colors[1].z);
        glTranslatef(0,0,-1);
        fw_quad_put(-1,-1,2,2);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3ub(_colors[2].x,_colors[2].y,_colors[2].z);
        glTranslatef(0,1,0);
        glRotatef(90,1,0,0);
        fw_quad_put(-1,-1,2,2);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3ub(_colors[3].x,_colors[3].y,_colors[3].z);
        glTranslatef(0,-1,0);
        glRotatef(90,1,0,0);
        fw_quad_put(-1,-1,2,2);
    }
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
}

void renderTwister(fw_timer_data *time) {
    fx_knit_viewport(0, 0);
    fx_knit_projection_perspective();

    glTranslatef(0,2.0,-10);

    int scissorWidth=3;
    glEnable(GL_SCISSOR_TEST);
    for (int i=0; i<FX_KNIT_CANVAS_WIDTH; i+=scissorWidth) {

        scissorWidth++;
        if (scissorWidth > 4) {
            scissorWidth = 2;
        }
        glScissor(i,0, scissorWidth, FX_KNIT_CANVAS_HEIGHT);

        const float a = (float)i/FX_KNIT_CANVAS_WIDTH;
        glPushMatrix();

        glTranslatef(0,.75*sinf(a*4 + time->elapsed*2) + .3f*cosf(a*4 + time->elapsed*4),0);
        glRotatef(3*240 * sinf(a+ time->elapsed*.5),1,0,0);
        glScalef(1,1.25f + sinf(a * 2 + time->elapsed),1);

        renderTwisterCube();
        glPopMatrix();

    }
    glDisable(GL_SCISSOR_TEST);
}

void fx_twister_render(float stencilX, fw_timer_data *time) {
    fw_stencil_begin();
    fw_stencil_activateStencilBuffer();

    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef(stencilX, 0, 0);

    fw_image *img = &getImages()[RES_IMG_BAYER_VERTICAL];
    fx_knit_imageBegin(img);
    glTranslatef(img->width,0,0);
    glScalef(-1,1,1);
    fw_image_putImageExt(img,1,0,11,0);
    fx_knit_imageEnd();

    fx_knit_imageBegin(img);
    glTranslatef(FX_KNIT_CANVAS_WIDTH + img->width,0,0);
    fw_image_putImageExt(img,1,0,11,0);
    fx_knit_imageEnd();

    glTranslatef(img->width,0,0);
    fw_quad_put(0,0,FX_KNIT_CANVAS_WIDTH,FX_KNIT_CANVAS_HEIGHT);

    glPopMatrix();

    fw_stencil_activateColorBuffer(0);
    renderTwister(time);

    fw_stencil_end();

    fx_knit_projection_ortho();
}
