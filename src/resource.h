#ifndef FW_RESOURCE_H
#define FW_RESOURCE_H

#include "fw/font.h"
#include "fw/image.h"

enum ImagesEnum {
    RES_IMG_FONT,
    RES_IMG_KNIT,
    RES_IMG_LOGO_SPECTROX,
    RES_IMG_LOGO_DESIRE,
    RES_IMG_BG_1,
    RES_IMG_FRAME_1,
    RES_IMG_FRAME_PATTERN_1,
    RES_IMG_DEER_1,
    RES_IMG_DEER_1_MONO,
    RES_IMG_DEER_2,
    RES_IMG_DEER_2_MONO,
    RES_IMG_TREE,
    RES_IMG_TREE_MONO,
    RES_IMG_TREE_STARS,
    RES_IMG_LOGO_GRADIENT,
    RES_IMG_FLAME_GRADIENT,
    RES_IMG_SANTA_WALK_1,
    RES_IMG_SANTA_WALK_2,
    RES_IMG_SANTA_STAND_1,
    RES_IMG_SANTA_STAND_2,
    RES_IMG_BAYER_VERTICAL,
    RES_IMG_BEAM,

    // See https://stackoverflow.com/a/11090007/891846
    IMAGES_ENUM_SIZE
};

void fw_resource_init();

fw_image *getImages();
fw_font_face *getFontFace();

#endif
