#include "knit.h"
#include <GL/glu.h>
#include "../fw/render.h"
#include "../resource.h"

#define FX_KNIT_CANVAS_BPP 4

#define KNIT_WIDTH 7
#define KNIT_HEIGHT 6

static const int _canvasPixelsLength = FX_KNIT_CANVAS_WIDTH * FX_KNIT_CANVAS_HEIGHT * FX_KNIT_CANVAS_BPP;
static const float _aspect = FX_KNIT_CANVAS_WIDTH/(float)FX_KNIT_CANVAS_HEIGHT;
static fw_image _canvasImage;

typedef struct {
    int numFaceIndices;
    int *faceIndices;
    float *vertices;
    unsigned char *colors;
    float *textureCoords;

} knit_arrays;

static knit_arrays _knitArrays = {0};

void fx_knit_init(void) {
    unsigned char *buffer = calloc(_canvasPixelsLength, sizeof(unsigned char));
    fw_image_initRGBA(&_canvasImage, FX_KNIT_CANVAS_WIDTH, FX_KNIT_CANVAS_HEIGHT, buffer);

    const int numFaces = FX_KNIT_CANVAS_WIDTH * FX_KNIT_CANVAS_HEIGHT;
    const int numFaceIndices = numFaces * 4;
    const int numVertices = numFaces * 4 * 2;
    const int numColors = numVertices * 4;
    const int numTextureCoords = numVertices * 2;

    _knitArrays.numFaceIndices = numFaceIndices;
    _knitArrays.vertices = calloc(numVertices, sizeof(float));
    _knitArrays.faceIndices = calloc(numFaceIndices, sizeof(int));
    _knitArrays.colors = calloc(numColors, sizeof(unsigned char));
    _knitArrays.textureCoords = calloc(numTextureCoords, sizeof(float));

    const int texW = getImages()[RES_IMG_KNIT].width;
    const int texH = getImages()[RES_IMG_KNIT].height;

    int idx = 0;
    for (int y=0; y<FX_KNIT_CANVAS_HEIGHT; y++) {
        for (int x=0; x<FX_KNIT_CANVAS_WIDTH; x++) {
            _knitArrays.vertices[idx++] = 0 + x*KNIT_WIDTH;
            _knitArrays.vertices[idx++] = 0 + y*KNIT_HEIGHT;

            _knitArrays.vertices[idx++] = 0 + x*KNIT_WIDTH;
            _knitArrays.vertices[idx++] = texH + y*KNIT_HEIGHT;

            _knitArrays.vertices[idx++] = texW + x*KNIT_WIDTH;
            _knitArrays.vertices[idx++] = texH + y*KNIT_HEIGHT;

            _knitArrays.vertices[idx++] = texW + x*KNIT_WIDTH;
            _knitArrays.vertices[idx++] = 0 + y*KNIT_HEIGHT;
        }
    }

    for (int i=0; i<numFaceIndices; i++) {
        _knitArrays.faceIndices[i] = i;
    }

    idx = 0;
    for (int y=0; y<FX_KNIT_CANVAS_HEIGHT; y++) {
        for (int x=0; x<FX_KNIT_CANVAS_WIDTH; x++) {
            _knitArrays.textureCoords[idx++] = 0;
            _knitArrays.textureCoords[idx++] = 0;

            _knitArrays.textureCoords[idx++] = 0;
            _knitArrays.textureCoords[idx++] = 1;

            _knitArrays.textureCoords[idx++] = 1;
            _knitArrays.textureCoords[idx++] = 1;

            _knitArrays.textureCoords[idx++] = 1;
            _knitArrays.textureCoords[idx++] = 0;
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void fx_knit_viewport(int xOffset, int yOffset) {
    glViewport(xOffset, yOffset, FX_KNIT_CANVAS_WIDTH, FX_KNIT_CANVAS_HEIGHT);
}

void fx_knit_readPixels(void) {

    // Grab rendered pixels.
    glReadPixels(0, 0, FX_KNIT_CANVAS_WIDTH, FX_KNIT_CANVAS_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, _canvasImage.pixels);

    // Copy to color array where every vertex of a quad needs to have a color, i.e. copy the same color value 4 times.
    int idx=0;
    for (int i=0; i<_canvasPixelsLength; i+=4) {
        for (int j=0; j<4; j++) {
            _knitArrays.colors[idx++] = _canvasImage.pixels[i];
            _knitArrays.colors[idx++] = _canvasImage.pixels[i+1];
            _knitArrays.colors[idx++] = _canvasImage.pixels[i+2];
            _knitArrays.colors[idx++] = _canvasImage.pixels[i+3];
        }
    }
}

void fx_knit_projection_ortho(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // `glReadPixels` puts origin to bottom-left.
    // Choose ortho projection so that origin is top-left.
    gluOrtho2D(0, FX_KNIT_CANVAS_WIDTH, 0, FX_KNIT_CANVAS_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void fx_knit_projection_perspective(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, _aspect, 0.01f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // `glReadPixels` puts origin to bottom-left.
    // Flip vertically to make it top-left.
    glScalef(1,-1,1);
}

void fx_knit_render(fw_timer_data *time) {
    fw_render_viewportHiRes(0,0);
    fw_render_clearHiRes(0,0,0,1);

    fw_image_renderBegin(&getImages()[RES_IMG_KNIT], 1);

    glVertexPointer(2, GL_FLOAT,0, _knitArrays.vertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, _knitArrays.colors);
    glTexCoordPointer(2, GL_FLOAT, 0, _knitArrays.textureCoords);

    glDrawElements(GL_QUADS, _knitArrays.numFaceIndices, GL_UNSIGNED_INT, _knitArrays.faceIndices);

    fw_image_renderEnd();
}

void fx_knit_imageBegin(fw_image *image) {
    glColor4f(1,1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image->textureID);
    glPushMatrix();
}

void fx_knit_imageEnd(void) {
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
