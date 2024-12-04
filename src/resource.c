#include "resource.h"
#include "fw/pixels.h"
#include "fw/draw.h"
#include "dat/fonts/font_pixels.h"
#include "dat/fonts/font_meta.h"
#include "dat/images/knit_pixels.h"
#include "dat/images/bayer_vertical_pixels.h"
#include "dat/images/bg_1_pixels.h"
#include "dat/images/deer_1_pixels.h"
#include "dat/images/deer_2_pixels.h"
#include "dat/images/frame_1_pixels.h"
#include "dat/images/frame_pattern_1_pixels.h"
#include "dat/images/logo_spectrox_pixels.h"
#include "dat/images/logo_desire_pixels.h"
#include "dat/images/tree_pixels.h"
#include "dat/images/tree_stars_pixels.h"
#include "dat/images/santa_walk_1_pixels.h"
#include "dat/images/santa_walk_2_pixels.h"
#include "dat/images/santa_stand_1_pixels.h"
#include "dat/images/santa_stand_2_pixels.h"

static fw_image _images[IMAGES_ENUM_SIZE];

static fw_vec3i _logoGradientColors[] = {
    {255, 255, 255},
    {240, 240, 240},
    {224, 224, 224},
    {209, 209, 209},
    {224, 224, 224},
    {224, 224, 224},
    {255, 255, 255},
    {240, 240, 240},
};
static int _logoGradientColorsLength = LEN(_logoGradientColors);

static fw_vec3i _flameGradientColors[] = {
    {255, 220, 5},
    {249, 150, 0},
    {255, 220, 5},
    {255, 169, 62},
    {255, 100, 0},
    {244, 148, 0},
};
static int _flameGradientColorsLength = LEN(_flameGradientColors);


static void generateImages(void) {
    fw_vec4i color = {255,255,0,255};
    int w, h, bpp=4;
    unsigned char *buffer;

    // RES_IMG_LOGO_GRADIENT.
    w = 1, h = _logoGradientColorsLength, buffer = calloc(w*h*bpp, sizeof(unsigned char));
    for (int i=0; i<_logoGradientColorsLength; i++) {
        color.x = _logoGradientColors[i].x;
        color.y = _logoGradientColors[i].y;
        color.z = _logoGradientColors[i].z;
        color.w = 255;
        fw_draw_pixel(0,i,&color,buffer,w,h,bpp);
    }
    fw_image_initRGBA(&_images[RES_IMG_LOGO_GRADIENT], w, h, buffer);

    // RES_IMG_FLAME_GRADIENT.
    w = 1, h = _flameGradientColorsLength, buffer = calloc(w*h*bpp, sizeof(unsigned char));
    for (int i=0; i<_flameGradientColorsLength; i++) {
        color.x = _flameGradientColors[i].x;
        color.y = _flameGradientColors[i].y;
        color.z = _flameGradientColors[i].z;
        color.w = 255;
        fw_draw_pixel(0,i,&color,buffer,w,h,bpp);
    }
    fw_image_initRGBA(&_images[RES_IMG_FLAME_GRADIENT], w, h, buffer);


    // RES_IMG_BEAM.
    // Build horizontal beam. White in center and black at the left/right edges.
    w = 128, h = 128, buffer = calloc(w*h*bpp, sizeof(unsigned char));
    const int numSegsments = 11;
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {

            int currentSegmentIdx = numSegsments * x / w;
            int centerSegmentIdx = numSegsments/2;
            int distToCenterSegment = (centerSegmentIdx - currentSegmentIdx);
            distToCenterSegment = distToCenterSegment < 0 ? -distToCenterSegment : distToCenterSegment;
            int intensity = 255 - 255.f * distToCenterSegment/(float)centerSegmentIdx;

            color = (fw_vec4i) {
                intensity,intensity,intensity,255
            };

            fw_draw_pixel(x,y,&color,buffer,w,h,bpp);
        }
    }
    fw_image_initRGBA(&_images[RES_IMG_BEAM], w, h, buffer);


    // RES_IMG_TREE_MONO.
    w = TREE_PIXELS_WIDTH, h = TREE_PIXELS_HEIGHT, buffer = calloc(w*h*bpp, sizeof(unsigned char));
    fw_pixels_createMask(_treePixels, buffer, w, h, bpp);
    fw_image_initRGBA(&_images[RES_IMG_TREE_MONO], w, h, buffer);

    // RES_IMG_DEER_1_MONO.
    w = DEER_1_PIXELS_WIDTH, h = DEER_1_PIXELS_HEIGHT, buffer = calloc(w*h*bpp, sizeof(unsigned char));
    fw_pixels_createMask(_deer1Pixels, buffer, w, h, bpp);
    fw_image_initRGBA(&_images[RES_IMG_DEER_1_MONO], w, h, buffer);

    // RES_IMG_DEER_2_MONO.
    w = DEER_2_PIXELS_WIDTH, h = DEER_2_PIXELS_HEIGHT, buffer = calloc(w*h*bpp, sizeof(unsigned char));
    fw_pixels_createMask(_deer2Pixels, buffer, w, h, bpp);
    fw_image_initRGBA(&_images[RES_IMG_DEER_2_MONO], w, h, buffer);

}

static void initImages(void) {
    fw_image_setTextureWrapParamClamp();
    fw_image_initRGBA(&_images[RES_IMG_KNIT], KNIT_PIXELS_WIDTH, KNIT_PIXELS_HEIGHT, _knitPixels);
    fw_image_initRGBA(&_images[RES_IMG_BG_1], BG_1_PIXELS_WIDTH, BG_1_PIXELS_HEIGHT, _bg1Pixels);
    fw_image_initRGBA(&_images[RES_IMG_DEER_1], DEER_1_PIXELS_WIDTH, DEER_1_PIXELS_HEIGHT, _deer1Pixels);
    fw_image_initRGBA(&_images[RES_IMG_DEER_2], DEER_2_PIXELS_WIDTH, DEER_2_PIXELS_HEIGHT, _deer2Pixels);
    fw_image_initRGBA(&_images[RES_IMG_FRAME_1], FRAME_1_PIXELS_WIDTH, FRAME_1_PIXELS_HEIGHT, _frame1Pixels);
    fw_image_initRGBA(&_images[RES_IMG_FRAME_PATTERN_1], FRAME_PATTERN_1_PIXELS_WIDTH, FRAME_PATTERN_1_PIXELS_HEIGHT, _framePattern1Pixels);
    fw_image_initRGBA(&_images[RES_IMG_LOGO_SPECTROX], LOGO_SPECTROX_PIXELS_WIDTH, LOGO_SPECTROX_PIXELS_HEIGHT, _logoSpectroxPixels);
    fw_image_initRGBA(&_images[RES_IMG_LOGO_DESIRE], LOGO_DESIRE_PIXELS_WIDTH, LOGO_DESIRE_PIXELS_HEIGHT, _logoDesirePixels);
    fw_image_initRGBA(&_images[RES_IMG_TREE], TREE_PIXELS_WIDTH, TREE_PIXELS_HEIGHT, _treePixels);
    fw_image_initRGBA(&_images[RES_IMG_TREE_STARS], TREE_STARS_PIXELS_WIDTH, TREE_STARS_PIXELS_HEIGHT, _treeStarsPixels);
    fw_image_initRGBA(&_images[RES_IMG_SANTA_WALK_1], SANTA_WALK_1_PIXELS_WIDTH, SANTA_WALK_1_PIXELS_HEIGHT, _santaWalk1Pixels);
    fw_image_initRGBA(&_images[RES_IMG_SANTA_WALK_2], SANTA_WALK_2_PIXELS_WIDTH, SANTA_WALK_2_PIXELS_HEIGHT, _santaWalk2Pixels);
    fw_image_initRGBA(&_images[RES_IMG_SANTA_STAND_1], SANTA_STAND_1_PIXELS_WIDTH, SANTA_STAND_1_PIXELS_HEIGHT, _santaStand1Pixels);
    fw_image_initRGBA(&_images[RES_IMG_SANTA_STAND_2], SANTA_STAND_2_PIXELS_WIDTH, SANTA_STAND_2_PIXELS_HEIGHT, _santaStand2Pixels);
    fw_image_setTextureWrapParamRepeat();
}

static void initBayer(void) {
    fw_image_initRGBA(&_images[RES_IMG_BAYER_VERTICAL], BAYER_VERTICAL_PIXELS_WIDTH, BAYER_VERTICAL_PIXELS_HEIGHT, _bayerVerticalPixels);
    // Clamp currently active texture horizontally to avoid repeat artifacts when translating to non-integer positions.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
}


static void initFonts(void) {
    fw_image_initRGBA(&_images[RES_IMG_FONT], FONT_PIXELS_WIDTH, FONT_PIXELS_HEIGHT, _fontPixels);
    fw_font_initFontFace(&_fontFace, &_images[RES_IMG_FONT]);
}

void fw_resource_init() {
    // Images.
    initImages();
    initBayer();
    initFonts();
    generateImages();
}

fw_image *getImages() {
    return _images;
}

fw_font_face *getFontFace() {
    return &_fontFace;
}
