#include "scene_0.h"
#include <GL/glu.h>
#include <math.h>
#include "config/config.h"
#include "resource.h"
#include "fw/types.h"
#include "fw/render.h"
#include "fw/lut.h"
#include "fw/easing.h"
#include "fw/animation.h"
#include "fw/font.h"
#include "fw/projection.h"
#include "fw/sync.h"
#include "fw/math.h"
#include "fw/quad.h"
#include "fw/stencil.h"
#include "fw/sync.h"
#include "fw/broadcast.h"
#include "fx/anim_text.h"
#include "fx/knit.h"
#include "fx/twister.h"
#include "fx/snow.h"
#include "fx/ornaments.h"
#include "fx/path.h"
#include "fx/star_3d.h"
#include "fx/particles.h"
#include "fx/aurora.h"
#include "dat/paths/spectrox_paths.h"
#include "dat/paths/desire_paths.h"

#define COLOR_4UB_SHADOW 0,0,0,150
#define COLOR_3UB_WHITE_WITH_RED_TINT 255,226,223

#define LOGO_SPECTROX_POS 8,11
#define LOGO_DESIRE_POS 10,12

#define MUSIC_START_BEAT_NUMBER 480

static fw_sync_data _syncData;

static fw_object_3d _star;
static fx_particle_emitter _starTrailEmitter;
static fx_particle_emitter _snowEmitter;

static fx_snow_flake _snowFlakes0[16];
static fx_snow_flake _snowFlakes1[48];
static fx_snow_flake _snowFlakes2[256];
static fx_snow_flake _snowFlakes3[512];
static fx_snow_flake _snowFlakes4[2048];

typedef struct {
    fx_snow_flake *flakes;
    int numFlakes;
    int smallFlakesPercentage;
    int isRespawnEnabledCurrent;
    fw_vec4f isRespawnEnabledTarget;
    fw_vec4f isEnabled;
} snow_data;

static snow_data _snowData[] = {
    {
        .flakes = _snowFlakes0,
        .numFlakes = LEN(_snowFlakes0),
        .smallFlakesPercentage = 100,
        .isRespawnEnabledCurrent = 1,
        .isRespawnEnabledTarget = {1},
        .isEnabled = {1}
    },
    {
        .flakes = _snowFlakes1,
        .numFlakes = LEN(_snowFlakes1),
        .smallFlakesPercentage = 92,
        .isRespawnEnabledCurrent = 1,
        .isRespawnEnabledTarget = {1},
        .isEnabled = {1}
    },
    {
        .flakes = _snowFlakes2,
        .numFlakes = LEN(_snowFlakes2),
        .smallFlakesPercentage = 92,
        .isRespawnEnabledCurrent = 0,
        .isRespawnEnabledTarget = {0},
        .isEnabled = {0}
    },
    {
        .flakes = _snowFlakes3,
        .numFlakes = LEN(_snowFlakes3),
        .smallFlakesPercentage = 90,
        .isRespawnEnabledCurrent = 0,
        .isRespawnEnabledTarget = {0},
        .isEnabled = {0}
    },
    {
        .flakes = _snowFlakes4,
        .numFlakes = LEN(_snowFlakes4),
        .smallFlakesPercentage = 20,
        .isRespawnEnabledCurrent = 0,
        .isRespawnEnabledTarget = {0},
        .isEnabled = {0}
    },
};
static const int _snowDataLength = LEN(_snowData);

static fx_ornament _stars[] = {
    {85, 12},
    {5, 9},
    {19, 9},
    {30, 17},
    {89, 15},
    {91, 28},
    {79, 13},
    {64, 13},
    {59, 10},
    {9, 11},
    {46, 11},
    {85, 18},
    {14, 13},
    {7, 14},
    {33, 11},
    {39, 10},
    {24, 13},
    {90, 11},
    {6, 19},
    {92, 19},
    {72, 13},
    {69, 10},
    {73, 9},
    {35, 14},
};
static const int _starsLength = LEN(_stars);

static fx_ornament _candles[] = {
    {49,40},
    {46,45},
    {52,45},
    {47,48},
    {44,50},
    {54,50},
    {51,52},
    {42,54},
    {55,54},
    {45,55},
    {40,59},
    {57,58},
    {37,61},
    {61,61},
    {45,62},
    {54,63},
    {59,63},
    {40,64},
};
static const int _candlesLength = LEN(_candles);

// All fade palettes have 31 entries.
static fw_vec3i _blackBlueRedPalette1[] = {
    {0, 0, 0},
    {3, 0, 12},
    {6, 0, 24},
    {9, 1, 36},
    {23, 1, 34},
    {37, 1, 31},
    {52, 2, 29},
    {66, 2, 26},
    {80, 3, 24},
    {95, 3, 21},
    {109, 3, 19},
    {123, 4, 16},
    {138, 4, 14},
    {152, 5, 11},
    {166, 5, 9},
    {181, 6, 6}, // Idx 15: Day.
    {170, 7, 9},
    {159, 8, 12},
    {147, 9, 15},
    {136, 10, 18},
    {124, 11, 21},
    {113, 12, 24},
    {101, 13, 27},
    {90, 14, 31},
    {78, 15, 34},
    {67, 16, 37},
    {55, 17, 40},
    {44, 18, 43},
    {32, 19, 46},
    {21, 20, 49},
    {9, 22, 53}, // Idx 30: Night.
};

// All fade palettes have 31 entries.
static fw_vec3i _blackBlueRedPalette2[] = {
    {0, 0, 0},
    {3, 0, 9},
    {7, 0, 19},
    {11, 1, 29},
    {22, 1, 28},
    {33, 1, 26},
    {45, 2, 24},
    {56, 2, 22},
    {67, 3, 20},
    {79, 3, 18},
    {90, 3, 16},
    {101, 4, 14},
    {113, 4, 12},
    {124, 5, 10},
    {135, 5, 8},
    {147, 6, 6}, // Idx 15: Day.
    {138, 6, 8},
    {128, 6, 10},
    {119, 7, 13},
    {109, 7, 15},
    {99, 8, 18},
    {90, 8, 20},
    {80, 9, 23},
    {71, 9, 25},
    {61, 10, 28},
    {51, 10, 30},
    {42, 11, 33},
    {32, 11, 35},
    {23, 12, 38},
    {13, 12, 40},
    {3, 13, 43}, // Idx 30: Night.
};

// All fade palettes have 31 entries.
static fw_vec3i _blackWhitePalette[] = {
    {0, 0, 0},
    {17, 15, 14},
    {34, 30, 29},
    {51, 45, 44},
    {68, 60, 59},
    {85, 75, 74},
    {102, 90, 89},
    {119, 105, 104},
    {136, 120, 118},
    {153, 135, 133},
    {170, 150, 148},
    {187, 165, 163},
    {204, 180, 178},
    {221, 195, 193},
    {238, 210, 208},
    {255, 226, 223}, // Idx 15: White with red tint.
    {253, 226, 225},
    {251, 226, 227},
    {249, 226, 229},
    {247, 226, 231},
    {245, 226, 233},
    {243, 226, 235},
    {241, 226, 237},
    {238, 226, 240},
    {236, 226, 242},
    {234, 226, 244},
    {232, 226, 246},
    {230, 226, 248},
    {228, 226, 250},
    {226, 226, 252},
    {223, 226, 255}, // Idx 30: White with blue tint.
};

enum anim_names_enum {
    ANIM_START,
    ANIM_LOGO_SPECTROX_PATH,
    ANIM_LOGO_SPECTROX_BLINDS,
    ANIM_LOGO_DESIRE_PATH,
    ANIM_LOGO_DESIRE_BLINDS,
    ANIM_LOGO_ROTATION_DELAY,
    ANIM_LOGO_ROTATION,
    ANIM_LOGO_REVEAL_SPECTROX,
    ANIM_LOGO_REVEAL_DESIRE,
    ANIM_STAR_IN,
    ANIM_MAIN,
    ANIM_SCROLLER,
};

///////////////////////////////////////////////////////////////////////////////
// STRINGS
///////////////////////////////////////////////////////////////////////////////
const char _scrollText[] =
    "HO! HO! HO! "
    " . . .   "

    "PUT ON YOUR UGLY XMAS SWEATER AND POUR YOURSELF SOME EGGNOG! . . .   "
    "BECAUSE SPECTROX AND DESIRE ARE HERE TO WISH YOU HAPPY HOLIDAYS! . . .   "
    "CREDITS FOR THIS SNOWY PRODUCTION GO TO . . .   KNITTING BY OLYMPIAN, CAROLING BY SLAZE, DECORATING BY RAMONB5 . . .   "
    "WE SEND CHEERFUL SEASON'S GREETINGS TO . . .   "

    "ABYSS, ALCATRAZ, ANDROMEDA, ARTSTATE, ASD, ATE BIT, "
    "BITBENDAZ, BITSHIFTERS, BLOCKTRONICS, BONZAI, "
    "CALODOX, CENSOR DESIGN, COMPLEX, CONSPIRACY, "
    "DISASTER AREA, DUSKWAVE, ECHTZEIT, ENCORE, "
    "FAIRLIGHT, FARBRAUSCH, FROZAK, FUNKTION, "
    "GENESIS PROJECT, HAUJOBB, HORK, INSANE, ISTARI, "
    "LEMON., LOGICOMA, MELON, MFX, MINDLAPSE, MOODS PLATEAU, NAH-KOLOR, NOICE, "
    "OSMIUM PATH, OVERLANDERS, OXYGENE, OXYRON, "
    "PLAYPSYCO, PROXIMA, QUEBARIUM, RAZOR 1911, REBELS, RESISTANCE, RIFT, "
    "SCA, SCOOPEX, SLIPSTREAM, SMFX, SPACEBALLS, SPREADPOINT, "
    "TEADRINKER, TEK, TITAN, TPOLM, TRBL, TREPAAN, TRIAD, TRSI, TTE, VANTAGE "

    "            "
    ". . . . . . . . . "
    "WISHING ALL YOU PIXEL PUSHERS, CODE CRUNCHERS, AND TUNE TWISTERS A GLITCH-FREE 2025! "
    "            "
    ". . . . . . . . . "
    "SIGNING OFF AND RESTARTING SCROLLER "
    ". . . . . . . . . . . . "
    "            "
    "% % % % %"
    ;

///////////////////////////////////////////////////////////////////////////////
// INITIAL KEYFRAME VARIABLES
///////////////////////////////////////////////////////////////////////////////
static const fw_vec4f _snowFlakeColorInit = {255,255,255};
static const fw_vec4f _logoDesireStencilRotInit = {90};
static const fw_vec4f _logoSpectroxStencilRotInit = {-90};
static const fw_vec4f _scrollerPosInit = {FX_KNIT_CANVAS_WIDTH};
static const fw_vec4f _twisterStencilPosInit = {FX_KNIT_CANVAS_WIDTH};
static const fw_vec4f _twisterStencilPosFullyVisible = {-27}; // 27 is bayer texture width.
static const fw_vec4f _twisterStencilPosFinal = {-27*2-FX_KNIT_CANVAS_WIDTH};
static const fw_vec4f _logoSpectroxBlindsHeight1Init = {12};
static const fw_vec4f _logoSpectroxBlindsHeight2Init = {13};
static const fw_vec4f _logoDesireBlindsHeight1Init = {12};
static const fw_vec4f _logoDesireBlindsHeight2Init = {13};
static const fw_vec4f _deerPosDeltaInit = {30};
static const fw_vec4f _deerPosDeltaFinal = {0};
static const fw_vec4f _starPosInit = {0,40+15,-300};
static const fw_vec4f _starPosOffsetInit = {0};
static const fw_vec4f _starRotInit = {0};
static const fw_vec4f _snowTransitionColorInit = {COLOR_3UB_WHITE_WITH_RED_TINT,255};
static const fw_vec4f _snowTransitionColorFinal = {COLOR_3UB_WHITE_WITH_RED_TINT,0};

static const fw_vec4f _santaPosXMin = {-20};
static const fw_vec4f _santaPosXMax = {FX_KNIT_CANVAS_WIDTH};
static const fw_vec4f _santaPosYTop = {44};
static const fw_vec4f _santaPosYBottom = {64};

static const fw_vec4f _auroraAlphaInit = {0};

///////////////////////////////////////////////////////////////////////////////
// KEY FRAME VARIABLES
///////////////////////////////////////////////////////////////////////////////
static fw_vec4f _fadePalettePos = {0};
static fw_vec4f _deerPosDelta = _deerPosDeltaInit;
static fw_vec4f _snowTransitionColor = _snowTransitionColorInit;
static fw_vec4f _snowFlakeColor = _snowFlakeColorInit;
static fw_vec4f _logoDesireStencilRot = _logoDesireStencilRotInit;
static fw_vec4f _logoSpectroxStencilRot = _logoSpectroxStencilRotInit;
static fw_vec4f _scrollerPos = _scrollerPosInit;
static fw_vec4f _twisterStencilPos = _twisterStencilPosInit;

static fw_vec4f _logoSpectroxBlindsHeight1 = _logoSpectroxBlindsHeight1Init;
static fw_vec4f _logoSpectroxBlindsHeight2 = _logoSpectroxBlindsHeight2Init;

static fw_vec4f _logoDesireBlindsHeight1 = _logoDesireBlindsHeight1Init;
static fw_vec4f _logoDesireBlindsHeight2 = _logoDesireBlindsHeight2Init;

static fw_vec4f _logoSpectroxPathPercentages[SPECTROX_PATH_ENUM_SIZE] = {0};
static fw_vec4f _logoDesirePathPercentages[DESIRE_PATH_ENUM_SIZE] = {0};

static fw_vec4f _starPos = _starPosInit;
static fw_vec4f _starPosOffset = _starPosOffsetInit;
static fw_vec4f _starRot = _starRotInit;

static fw_vec4f _snowEmitterRot = {0};
static fw_vec4f _starsPercentage = {0};

static fw_vec4f _santaPosX = _santaPosXMin;
static fw_vec4f _santaPosY = _santaPosYTop;

static fw_vec4f _auroraAlpha = _auroraAlphaInit;

///////////////////////////////////////////////////////////////////////////////
// ANIM START / STOP DECLARATIONS
///////////////////////////////////////////////////////////////////////////////
static void animStartedStart(fw_animation *anim, fw_timer_data *time);
static void animCompletedStart(fw_animation *anim, fw_timer_data *time);

static void animStartedLogoSpectroxPath(fw_animation *anim, fw_timer_data *time);
static void animCompletedLogoSpectroxPath(fw_animation *anim, fw_timer_data *time);

static void animStartedLogoSpectroxBlinds(fw_animation *anim, fw_timer_data *time);
static void animCompletedLogoSpectroxBlinds(fw_animation *anim, fw_timer_data *time);

static void animStartedLogoDesireBlinds(fw_animation *anim, fw_timer_data *time);
static void animCompletedLogoDesireBlinds(fw_animation *anim, fw_timer_data *time);

static void animStartedLogoDesirePath(fw_animation *anim, fw_timer_data *time);
static void animCompletedLogoDesirePath(fw_animation *anim, fw_timer_data *time);

static void animStartedStarIn(fw_animation *anim, fw_timer_data *time);
static void animCompletedStarIn(fw_animation *anim, fw_timer_data *time);

static void animStartedLogoRotationDelay(fw_animation *anim, fw_timer_data *time);
static void animCompletedLogoRotationDelay(fw_animation *anim, fw_timer_data *time);

static void animStartedLogoRotation(fw_animation *anim, fw_timer_data *time);
static void animCompletedLogoRotation(fw_animation *anim, fw_timer_data *time);

static void animStartedMain(fw_animation *anim, fw_timer_data *time);
static void animCompletedMain(fw_animation *anim, fw_timer_data *time);

static void animStartedScroller(fw_animation *anim, fw_timer_data *time);
static void animCompletedScroller(fw_animation *anim, fw_timer_data *time);

///////////////////////////////////////////////////////////////////////////////
// KEY FRAME CALLBACKS
///////////////////////////////////////////////////////////////////////////////

static void startStarTrailEmitter(fw_timer_data *time) {
    _starTrailEmitter.isParticleSpawnEnabled = 1;
}

static void startSnowEmitter(fw_timer_data *time) {
    _snowEmitter.isParticleSpawnEnabled = 1;
}

static void stopSnowEmitter(fw_timer_data *time) {
    _snowEmitter.isParticleSpawnEnabled = 0;
}

static void startLogoRevealSpectrox(fw_timer_data *time);
static void startLogoRevealDesire(fw_timer_data *time);

///////////////////////////////////////////////////////////////////////////////
// KEY FRAMES
///////////////////////////////////////////////////////////////////////////////

static fw_keyframe _keysStart[] = {
    {4, 6, {0}, {15}, LinearInterpolation, &_fadePalettePos},
    {4, 6, _snowFlakeColorInit, {COLOR_3UB_WHITE_WITH_RED_TINT}, LinearInterpolation, &_snowFlakeColor},

    // Artificial wait.
    {10, 10, {0}, {0}},
};

static fw_keyframe _keysLogoSpectroxPath[] = {
    {0, 2, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_S1]},
    {1, 2, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_P1]},
    {1.5, 2, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_E1]},
    {2, 2.5, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_E2]},
    {2.25, 2.75, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_E3]},
    {2.75, 3.25, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_E4]},
    {3, 4.25, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_C1]},
    {3.75, 4.75, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_T2]},
    {4.5, 5.5, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_T1]},
    {4.75, 6.75, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_R1]},
    {5.75, 6.75, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_O1]},
    {6, 7, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_X1]},
    {7, 7.75, {0}, {1}, LinearInterpolation, &_logoSpectroxPathPercentages[SPECTROX_PATH_X2]},
};

static fw_keyframe _keysLogoSpectroxBlinds[] = {
    {1.5, 2, _logoSpectroxBlindsHeight1Init, {50}, LinearInterpolation, &_logoSpectroxBlindsHeight1},
    {2, 2.5, _logoSpectroxBlindsHeight2Init, {50}, LinearInterpolation, &_logoSpectroxBlindsHeight2},
};

static fw_keyframe _keysLogoDesirePath[] = {
    {0, .5, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_D1_1]},
    {.5, 3, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_D1_2]},
    {1.75, 2.25, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E1_1]},
    {2.25, 2.75, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E1_2]},
    {2.5, 3, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E1_3]},
    {3, 3.5, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E1_4]},
    {3, 4.25, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_S1_1]},
    {3.75, 4.75, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_I1_1]},
    {7.25, 7.5, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_I1_2]},
    {4.25, 5, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_R1_1]},
    {4.75, 7.25, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_R1_2]},
    {5.5, 6, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E2_1]},
    {6, 6.5, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E2_2]},
    {6.25, 6.75, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E2_3]},
    {6.75, 7.25, {0}, {1}, LinearInterpolation, &_logoDesirePathPercentages[DESIRE_PATH_E2_4]},

    // Disable snow.
    {5,5, {0}, {0}, LinearInterpolation, &_snowData[0].isRespawnEnabledTarget},
    {5,5, {0}, {0}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},
};

static fw_keyframe _keysLogoDesireBlinds[] = {
    {1.5, 2, _logoDesireBlindsHeight1Init, {50}, LinearInterpolation, &_logoDesireBlindsHeight1},
    {2, 2.5, _logoDesireBlindsHeight2Init, {50}, LinearInterpolation, &_logoDesireBlindsHeight2},
};

static fw_keyframe _keysStarIn[] = {
    {1.75,10.65, {0}, {0,0,1,360*6}, LinearInterpolation, &_starRot},

    {1.25,3.25, _starPosInit, {0,5,-50}, QuadraticEaseInOut, &_starPos},
    {7.75,10.65, {0,5,-50}, {0,0,-1.4}, QuadraticEaseInOut, &_starPos},

    {4.75,6.75, _starPosOffsetInit, {12,8}, QuadraticEaseInOut, &_starPosOffset},
    {7.75,9.75, {12,8}, _starPosOffsetInit, QuadraticEaseInOut, &_starPosOffset},

    {5.25, 5.25, {0}, {0}, NULL, NULL, startStarTrailEmitter},
};

static fw_keyframe _keysLogoRotationDelay[] = {
    {0,0, {1}, {1}, LinearInterpolation, &_snowData[0].isRespawnEnabledTarget},
    {0,0, {1}, {1}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},
    {25,25, {0}, {0}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},

    // Artificial pause.
    {26.8, 26.8, {0}, {0}},
};

static fw_keyframe _keysLogoRevealSpectrox[] = {
    {.5,   0,       {0}, {0}, NULL, NULL, startSnowEmitter},
    {.5,   12.5,    {-75}, {90}, LinearInterpolation, &_snowEmitterRot},
    {11,   0,       {0}, {0}, NULL, NULL, stopSnowEmitter},

    {0,  12,        _logoDesireStencilRotInit, {270}, LinearInterpolation, &_logoDesireStencilRot},
    {.1, 12.1,      _logoSpectroxStencilRotInit, {90}, LinearInterpolation, &_logoSpectroxStencilRot},
};

static fw_keyframe _keysLogoRevealDesire[] = {
    {.5,   0,       {0}, {0}, NULL, NULL, startSnowEmitter},
    {.5,   12.5,    {-75}, {90}, LinearInterpolation, &_snowEmitterRot},
    {11,   0,       {0}, {0}, NULL, NULL, stopSnowEmitter},

    {.1, 12.1,      {-90}, {90}, LinearInterpolation, &_logoDesireStencilRot},
    {0,  12,        {90}, {270}, LinearInterpolation, &_logoSpectroxStencilRot},
};

static fw_keyframe _keysLogoRotation[] = {
    {0,0, {15}, {15}, LinearInterpolation, &_fadePalettePos},

    // Spectrox.
    {0,0, {0}, {0}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},
    {0, 0, {0}, {0}, NULL, NULL, startLogoRevealSpectrox},
    {8,8, {1}, {1}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},

    // Twister in.
    {14,14, {0}, {0}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},
    {15,19, _twisterStencilPosInit, _twisterStencilPosFullyVisible, LinearInterpolation, &_twisterStencilPos},

    // Desire.
    {23,23, {0}, {0}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},
    {24, 0, {0}, {0}, NULL, NULL, startLogoRevealDesire},
    {30,30, {1}, {1}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},

    // Spectrox.
    {47.5,47.5, {0}, {0}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},
    {48,0, {0}, {0}, NULL, NULL, startLogoRevealSpectrox},
    {58,58, {1}, {1}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},

    // Twister out.
    {62,66, _twisterStencilPosFullyVisible, _twisterStencilPosFinal, LinearInterpolation, &_twisterStencilPos},
    {62,62, {1}, {1}, LinearInterpolation, &_snowData[1].isRespawnEnabledTarget},

    // Day -> night.
    {66,69, {15}, {30}, LinearInterpolation, &_fadePalettePos},
    {68,71, {0}, {1}, LinearInterpolation, &_starsPercentage},

    // Desire.
    {72,0, {0}, {0}, NULL, NULL, startLogoRevealDesire},

    // Spectrox.
    {96,0, {0}, {0}, NULL, NULL, startLogoRevealSpectrox},

    // Desire.
    {120, 0, {0}, {0}, NULL, NULL, startLogoRevealDesire},

    // Santa top.
    {86,86, _santaPosYTop, _santaPosYTop, LinearInterpolation, &_santaPosY},
    {86,93.4, _santaPosXMin, {55}, LinearInterpolation, &_santaPosX},
    {101,103.4, {55}, {30}, LinearInterpolation, &_santaPosX},
    {108,110.4, {30}, {55}, LinearInterpolation, &_santaPosX},
    {114,116.4, {55}, {30}, LinearInterpolation, &_santaPosX},
    {122,127, {30}, _santaPosXMin, LinearInterpolation, &_santaPosX},

    // Santa bottom.
    {128,128, _santaPosYBottom, _santaPosYBottom, LinearInterpolation, &_santaPosY},
    {128,132, _santaPosXMin, {22}, LinearInterpolation, &_santaPosX},
    {136,138.4, {22}, {50}, LinearInterpolation, &_santaPosX},
    {144,147, {50}, {20}, LinearInterpolation, &_santaPosX},
    {152,158.6, {20}, _santaPosXMax, LinearInterpolation, &_santaPosX},

    // Spectrox.
    {144, 0, {0}, {0}, NULL, NULL, startLogoRevealSpectrox},

    // Fade in aurora.
    {160,163, _auroraAlphaInit, {1}, LinearInterpolation, &_auroraAlpha},
    // Fade out stars.
    {158,161, {1}, {0}, LinearInterpolation, &_starsPercentage},

    // Desire.
    {168, 0, {0}, {0}, NULL, NULL, startLogoRevealDesire},

    // Spectrox.
    {192, 0, {0}, {0}, NULL, NULL, startLogoRevealSpectrox},

    // Final Santa.
    {205,205, _santaPosYTop, _santaPosYTop, LinearInterpolation, &_santaPosY},
    {205,217, _santaPosXMax, _santaPosXMin, LinearInterpolation, &_santaPosX},

    // Snow storm on.
    {182,182, {1}, {1}, LinearInterpolation, &_snowData[2].isRespawnEnabledTarget},
    {182,182, {1}, {1}, LinearInterpolation, &_snowData[2].isEnabled},
    {184,184, {1}, {1}, LinearInterpolation, &_snowData[3].isRespawnEnabledTarget},
    {184,184, {1}, {1}, LinearInterpolation, &_snowData[3].isEnabled},

    // Snow storm off.
    {183,183, {0}, {0}, LinearInterpolation, &_snowData[2].isRespawnEnabledTarget},
    {212,212, {0}, {0}, LinearInterpolation, &_snowData[2].isEnabled},
    {185,185, {0}, {0}, LinearInterpolation, &_snowData[3].isRespawnEnabledTarget},
    {214,214, {0}, {0}, LinearInterpolation, &_snowData[3].isEnabled},

    // Desire.
    {216, 0, {0}, {0}, NULL, NULL, startLogoRevealDesire},

    // Fade out aurora.
    {232,235, {1}, _auroraAlphaInit, LinearInterpolation, &_auroraAlpha},
    // Fade in stars.
    {233,234, {0}, {1}, LinearInterpolation, &_starsPercentage},
    // Night -> day.
    {236,239, {30}, {15}, LinearInterpolation, &_fadePalettePos},
    // Fade out stars.
    {235,237, {1}, {0}, LinearInterpolation, &_starsPercentage},
};

static fw_keyframe _keysMain[] = {
    {0, 1, _snowTransitionColorInit, _snowTransitionColorFinal, LinearInterpolation, &_snowTransitionColor},

    {0, 1, {COLOR_3UB_WHITE_WITH_RED_TINT}, {255,255,255}, LinearInterpolation, &_snowFlakeColor},

    {0,0, {1}, {1}, LinearInterpolation, &_snowData[4].isEnabled},
    {1,1, {0}, {0}, LinearInterpolation, &_snowData[4].isRespawnEnabledTarget},
    {30,30, {0}, {0}, LinearInterpolation, &_snowData[4].isEnabled},

    // Main scene never ends.
    {99999999, 99999999, {0}, {0}},
};

static fw_keyframe _keysScroller[] = {
    {0, 507, _scrollerPosInit, {-5630}, LinearInterpolation, &_scrollerPos},
};

///////////////////////////////////////////////////////////////////////////////
// ANIMS
///////////////////////////////////////////////////////////////////////////////

static fw_animation _anims[] = {
    [ANIM_START] = {
        .isAutostart = 1,
        .keyframes = _keysStart,
        .numKeyframes = LEN(_keysStart),
        .started_proc = animStartedStart,
        .completed_proc = animCompletedStart,
    },
    [ANIM_MAIN] = {
        .isAutostart = 0,
        .keyframes = _keysMain,
        .numKeyframes = LEN(_keysMain),
        .started_proc = animStartedMain,
        .completed_proc = animCompletedMain,
    },
    [ANIM_LOGO_SPECTROX_PATH] = {
        .isAutostart = 0,
        .keyframes = _keysLogoSpectroxPath,
        .numKeyframes = LEN(_keysLogoSpectroxPath),
        .started_proc = animStartedLogoSpectroxPath,
        .completed_proc = animCompletedLogoSpectroxPath,
    },
    [ANIM_LOGO_SPECTROX_BLINDS] = {
        .isAutostart = 0,
        .keyframes = _keysLogoSpectroxBlinds,
        .numKeyframes = LEN(_keysLogoSpectroxBlinds),
        .started_proc = animStartedLogoSpectroxBlinds,
        .completed_proc = animCompletedLogoSpectroxBlinds,
    },
    [ANIM_LOGO_DESIRE_BLINDS] = {
        .isAutostart = 0,
        .keyframes = _keysLogoDesireBlinds,
        .numKeyframes = LEN(_keysLogoDesireBlinds),
        .started_proc = animStartedLogoDesireBlinds,
        .completed_proc = animCompletedLogoDesireBlinds,
    },
    [ANIM_LOGO_DESIRE_PATH] = {
        .isAutostart = 0,
        .keyframes = _keysLogoDesirePath,
        .numKeyframes = LEN(_keysLogoDesirePath),
        .started_proc = animStartedLogoDesirePath,
        .completed_proc = animCompletedLogoDesirePath,
    },
    [ANIM_STAR_IN] = {
        .isAutostart = 0,
        .keyframes = _keysStarIn,
        .numKeyframes = LEN(_keysStarIn),
        .started_proc = animStartedStarIn,
        .completed_proc = animCompletedStarIn,
    },
    [ANIM_LOGO_ROTATION] = {
        .isAutostart = 0,
        .keyframes = _keysLogoRotation,
        .numKeyframes = LEN(_keysLogoRotation),
        .started_proc = animStartedLogoRotation,
        .completed_proc = animCompletedLogoRotation,
    },
    [ANIM_LOGO_ROTATION_DELAY] = {
        .isAutostart = 0,
        .keyframes = _keysLogoRotationDelay,
        .numKeyframes = LEN(_keysLogoRotationDelay),
        .started_proc = animStartedLogoRotationDelay,
        .completed_proc = animCompletedLogoRotationDelay,
    },
    [ANIM_LOGO_REVEAL_SPECTROX] = {
        .isAutostart = 0,
        .keyframes = _keysLogoRevealSpectrox,
        .numKeyframes = LEN(_keysLogoRevealSpectrox),
    },
    [ANIM_LOGO_REVEAL_DESIRE] = {
        .isAutostart = 0,
        .keyframes = _keysLogoRevealDesire,
        .numKeyframes = LEN(_keysLogoRevealDesire),
    },
    [ANIM_SCROLLER] = {
        .isAutostart = 0,
        .keyframes = _keysScroller,
        .numKeyframes = LEN(_keysScroller),
        .started_proc = animStartedScroller,
        .completed_proc = animCompletedScroller,
    },
};

static fw_animation_schedule _animSchedule = {
    _anims,
    LEN(_anims)
};

static BOOL isRunning(enum anim_names_enum animNameEnum) {
    return _animSchedule.animations[animNameEnum].state == RUNNING;
}

static void startLogoRevealSpectrox(fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_REVEAL_SPECTROX], time);
}

static void startLogoRevealDesire(fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_REVEAL_DESIRE], time);
}


///////////////////////////////////////////////////////////////////////////////
// ANIM START / STOP IMPLEMENTATIONS
///////////////////////////////////////////////////////////////////////////////
static void animStartedStart(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedStart(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_SPECTROX_PATH], time);
}

static void animStartedLogoSpectroxPath(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedLogoSpectroxPath(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_SPECTROX_BLINDS], time);
}

static void animStartedLogoSpectroxBlinds(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedLogoSpectroxBlinds(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_DESIRE_PATH], time);
}

static void animStartedLogoDesirePath(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedLogoDesirePath(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_DESIRE_BLINDS], time);
}

static void animStartedLogoDesireBlinds(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_STAR_IN], time);
}
static void animCompletedLogoDesireBlinds(fw_animation *anim, fw_timer_data *time) {
}

static void animStartedStarIn(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedStarIn(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_MAIN], time);
}

static void animStartedLogoRotationDelay(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedLogoRotationDelay(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_ROTATION], time);
}

static void animStartedLogoRotation(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedLogoRotation(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_ROTATION], time);
}

static void animStartedMain(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_LOGO_ROTATION_DELAY], time);
    fw_animation_start(&_anims[ANIM_SCROLLER], time);
}
static void animCompletedMain(fw_animation *anim, fw_timer_data *time) {
}

static void animStartedScroller(fw_animation *anim, fw_timer_data *time) {
}
static void animCompletedScroller(fw_animation *anim, fw_timer_data *time) {
    fw_animation_start(&_anims[ANIM_SCROLLER], time);
}

///////////////////////////////////////////////////////////////////////////////
// MUSIC SYNC
///////////////////////////////////////////////////////////////////////////////
static enum ImagesEnum _syncDeerLeftImagesEnum = RES_IMG_DEER_1;
static enum ImagesEnum _syncDeerMonoLeftImagesEnum = RES_IMG_DEER_1_MONO;
static enum ImagesEnum _syncDeerRightImagesEnum = RES_IMG_DEER_2;
static enum ImagesEnum _syncDeerMonoRightImagesEnum = RES_IMG_DEER_2_MONO;

static enum ImagesEnum _syncSantaImagesEnum = RES_IMG_SANTA_WALK_1;

static int _isSongAmbiance = 1;

static void syncDeerAnim(void) {
    if (_syncDeerLeftImagesEnum == RES_IMG_DEER_1) {
        _syncDeerLeftImagesEnum = RES_IMG_DEER_2;
        _syncDeerMonoLeftImagesEnum = RES_IMG_DEER_2_MONO;
    } else {
        _syncDeerLeftImagesEnum = RES_IMG_DEER_1;
        _syncDeerMonoLeftImagesEnum = RES_IMG_DEER_1_MONO;
    }
    if (_syncDeerRightImagesEnum == RES_IMG_DEER_1) {
        _syncDeerRightImagesEnum = RES_IMG_DEER_2;
        _syncDeerMonoRightImagesEnum = RES_IMG_DEER_2_MONO;
    } else {
        _syncDeerRightImagesEnum = RES_IMG_DEER_1;
        _syncDeerMonoRightImagesEnum = RES_IMG_DEER_1_MONO;
    }
}


static float _prevSantaPosX = 0;
static void syncSantaAnim(void) {
    const int isWalking = _prevSantaPosX != _santaPosX.x;
    if (_syncSantaImagesEnum == RES_IMG_SANTA_WALK_1 || _syncSantaImagesEnum == RES_IMG_SANTA_STAND_1) {
        _syncSantaImagesEnum = isWalking ? RES_IMG_SANTA_WALK_2 : RES_IMG_SANTA_STAND_2;
    } else {
        _syncSantaImagesEnum = isWalking ? RES_IMG_SANTA_WALK_1 : RES_IMG_SANTA_STAND_1;
    }
    _prevSantaPosX = _santaPosX.x;
}

static void triggerSyncKick(int beatNumber, fw_timer_data *time) {
    if (_isSongAmbiance && _deerPosDelta.x <= _deerPosDeltaFinal.x) {
        syncDeerAnim();
    }
}

static void triggerSyncSnare(int beatNumber, fw_timer_data *time) {
}

static void triggerSyncKickOrSnare(int beatNumber, fw_timer_data *time) {
    if (_deerPosDelta.x > _deerPosDeltaFinal.x && time->elapsed > 8) {
        syncDeerAnim();
        _deerPosDelta.x -= 2;
    }

    if (!_isSongAmbiance && _deerPosDelta.x <= _deerPosDeltaFinal.x) {
        syncDeerAnim();
    }

    syncSantaAnim();
}

static void triggerSync(int beatNumber, fw_timer_data *time) {
    int isKick = beatNumber % 8 == 0;
    int isSnare = (beatNumber+4) % 8 == 0;

    if (_isSongAmbiance && beatNumber >= MUSIC_START_BEAT_NUMBER) {
        _isSongAmbiance = 0;
        fw_sync_modify(&_syncData, 150, 6, 1);
    }

    if (isKick) {
        triggerSyncKick(beatNumber, time);
    }

    if (isSnare) {
        triggerSyncSnare(beatNumber, time);
    }

    if (isKick || isSnare) {
        triggerSyncKickOrSnare(beatNumber, time);
    }
}


///////////////////////////////////////////////////////////////////////////////
// COMMON
///////////////////////////////////////////////////////////////////////////////

static fw_timer_data animTime(enum anim_names_enum animNameEnum, fw_timer_data *time) {
    fw_timer_data animTime = *time;
    animTime.elapsed = time->elapsed - _animSchedule.animations[animNameEnum].timeStartedAt;
    return animTime;
}

///////////////////////////////////////////////////////////////////////////////
// INIT
///////////////////////////////////////////////////////////////////////////////

static void initSnowData(void) {
    for (int i=0; i<_snowDataLength; i++) {
        fx_snow_init(_snowData[i].flakes, _snowData[i].numFlakes, _snowData[i].smallFlakesPercentage);
    }

    for (int i=0; i<LEN(_snowFlakes2); i++) {
        _snowFlakes2[i].speed *= 1.5;
    }

    for (int i=0; i<LEN(_snowFlakes3); i++) {
        _snowFlakes3[i].speed *= 2.0;
    }

    for (int i=0; i<LEN(_snowFlakes4); i++) {
        _snowFlakes4[i].y = fw_math_rand(-FX_KNIT_CANVAS_HEIGHT*.25, FX_KNIT_CANVAS_HEIGHT);
        _snowFlakes4[i].speed *= 1.5;
    }
}

static void updateSnowEmitterParticle(fx_particle *p, fx_particle_emitter *e, fw_timer_data *time) {
    p->pos.y += p->speed*time->delta;
    p->pos.x -= p->age*45*time->delta;
}

void scene_0_init() {
    fw_animation_resetAnimationSchedule(&_animSchedule);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);

    fx_knit_init();
    initSnowData();

    fx_star_3d_init(&_star);

    fx_particles_init(&_starTrailEmitter, 300, 2, .005f);
    _starTrailEmitter.particleSpawnPosVariance.x = 1.5f;
    _starTrailEmitter.particleSpawnPosVariance.y = .25f;
    _starTrailEmitter.particleSpawnSpeedVariance.x = 1;
    _starTrailEmitter.particleSpawnSpeedVariance.y = 4.5f;
    _starTrailEmitter.particleColor = (fw_vec4i) {
        COLOR_3UB_WHITE_WITH_RED_TINT, 255
    };
    _starTrailEmitter.gravity = 1;
    _starTrailEmitter.isParticleRenderEnabled = 1;


    fx_particles_init(&_snowEmitter, 200, 2, .0125f);
    _snowEmitter.particleSpawnPosVariance.x = 2;
    _snowEmitter.particleSpawnPosVariance.y = 4;
    _snowEmitter.particleSpawnSpeedVariance.x = 8;
    _snowEmitter.particleSpawnSpeedVariance.y = 16;
    _snowEmitter.particleColor = (fw_vec4i) {
        240,240,240, 255
    };
    _snowEmitter.gravity = 2;
    _snowEmitter.isParticleRenderEnabled = 1;
    _snowEmitter.updateParticleProc = updateSnowEmitterParticle;

    fx_aurora_init();

    // Set up music sync: 125 BPM, 4 SPEED. Trigger every row.
    fw_sync_init2(&_syncData, 125, 4, 1, triggerSync);
}

///////////////////////////////////////////////////////////////////////////////
// EXECUTE
///////////////////////////////////////////////////////////////////////////////

// +++++++++++++++++++ //
// SCENE: START
// +++++++++++++++++++ //

static void renderBG1(void) {
    glColor3ub(_blackBlueRedPalette1[(int)_fadePalettePos.x].x,
               _blackBlueRedPalette1[(int)_fadePalettePos.x].y,
               _blackBlueRedPalette1[(int)_fadePalettePos.x].z);
    fw_quad_put(0,0, FX_KNIT_CANVAS_WIDTH, FX_KNIT_CANVAS_HEIGHT);

    fx_knit_imageBegin(&getImages()[RES_IMG_BG_1]);
    glColor3ub(_blackBlueRedPalette2[(int)_fadePalettePos.x].x,
               _blackBlueRedPalette2[(int)_fadePalettePos.x].y,
               _blackBlueRedPalette2[(int)_fadePalettePos.x].z);
    fw_image_putImage(&getImages()[RES_IMG_BG_1]);
    fx_knit_imageEnd();
}

static void renderFrame1(void) {
    fx_knit_imageBegin(&getImages()[RES_IMG_FRAME_1]);
    glColor3ub(_blackWhitePalette[(int)_fadePalettePos.x].x,
               _blackWhitePalette[(int)_fadePalettePos.x].y,
               _blackWhitePalette[(int)_fadePalettePos.x].z);
    fw_image_putImage(&getImages()[RES_IMG_FRAME_1]);
    fx_knit_imageEnd();
}

static void renderFramePattern1(void) {
    fx_knit_imageBegin(&getImages()[RES_IMG_FRAME_PATTERN_1]);
    glColor3ub(_blackWhitePalette[(int)_fadePalettePos.x].x,
               _blackWhitePalette[(int)_fadePalettePos.x].y,
               _blackWhitePalette[(int)_fadePalettePos.x].z);
    glTranslatef(0,71,0);
    fw_image_putImage(&getImages()[RES_IMG_FRAME_PATTERN_1]);
    fx_knit_imageEnd();
}

static void renderTreeStars(void) {
    fx_knit_imageBegin(&getImages()[RES_IMG_TREE_STARS]);
    glColor3ub(_blackWhitePalette[(int)_fadePalettePos.x].x,
               _blackWhitePalette[(int)_fadePalettePos.x].y,
               _blackWhitePalette[(int)_fadePalettePos.x].z);
    glTranslatef(32,39,0);
    fw_image_putImage(&getImages()[RES_IMG_TREE_STARS]);
    fx_knit_imageEnd();
}

static void renderTree(enum ImagesEnum imagesEnum, int r, int g, int b) {
    fx_knit_imageBegin(&getImages()[imagesEnum]);
    glColor3ub(r,g,b);
    glTranslatef(36,40,0);
    fw_image_putImage(&getImages()[imagesEnum]);
    fx_knit_imageEnd();
}

static void renderDeer(int isMono, int r, int g, int b) {
    enum ImagesEnum imageLeftEnum = _syncDeerLeftImagesEnum;
    enum ImagesEnum imageRightEnum = _syncDeerRightImagesEnum;

    if (isMono) {
        imageLeftEnum = _syncDeerMonoLeftImagesEnum;
        imageRightEnum = _syncDeerMonoRightImagesEnum;
    }

    // Left deer.
    fx_knit_imageBegin(&getImages()[imageLeftEnum]);
    glColor3ub(r,g,b);
    glTranslatef(15-_deerPosDelta.x,49,0);
    fw_image_putImage(&getImages()[imageLeftEnum]);
    fx_knit_imageEnd();

    // Right deer.
    fx_knit_imageBegin(&getImages()[imageRightEnum]);
    glColor3ub(r,g,b);
    glTranslatef(84+_deerPosDelta.x,49,0);
    glScalef(-1,1,1);
    fw_image_putImage(&getImages()[imageRightEnum]);
    fx_knit_imageEnd();
}

static void renderSnowData(void) {
    for (int i=0; i<_snowDataLength; i++) {
        if (_snowData[i].isEnabled.x) {
            fx_snow_render(_snowData[i].flakes, _snowData[i].numFlakes, _snowFlakeColor.x,_snowFlakeColor.y,_snowFlakeColor.z);
        }
    }
}

static void renderStartScene(fw_timer_data *time) {
    fx_knit_viewport(0,0);
    fx_knit_projection_ortho();

    renderBG1();
    renderFrame1();
    renderFramePattern1();
    renderDeer(TRUE, COLOR_3UB_WHITE_WITH_RED_TINT);
    renderTree(RES_IMG_TREE_MONO,
               _blackWhitePalette[(int)_fadePalettePos.x].x,
               _blackWhitePalette[(int)_fadePalettePos.x].y,
               _blackWhitePalette[(int)_fadePalettePos.x].z);
    renderTreeStars();
}

// +++++++++++++++++++ //
// SCENE: PLAIN
// +++++++++++++++++++ //

static void renderLogo(enum ImagesEnum imagesEnum, int x, int y) {
    fx_knit_imageBegin(&getImages()[imagesEnum]);
    glColor3ub(COLOR_3UB_WHITE_WITH_RED_TINT);
    glTranslatef(x,y,0);
    fw_image_putImage(&getImages()[imagesEnum]);
    fx_knit_imageEnd();
}

static void renderLines(int yStart, int yEnd, int yInc, unsigned char r, unsigned char g, unsigned char b) {
    glColor3ub(r,g,b);
    glPushMatrix();
    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i=yStart; i<yEnd; i+=yInc) {
        glVertex2f(0, i);
        glVertex2f(FX_KNIT_CANVAS_WIDTH, i);
    }
    glEnd();
    glPopMatrix();
}

static void renderLogoBlinds(enum ImagesEnum imageEnum, int x, int y, int blindsY, int blindsEvenHeight, int blindsOddHeight) {
    fw_stencil_begin();
    fw_stencil_activateStencilBuffer();

    renderLines(blindsY, blindsEvenHeight, 2, 0,0,0);
    renderLines(blindsY+1, blindsOddHeight, 2, 0,0,0);

    fw_stencil_activateColorBuffer(1);
    renderLogo(imageEnum, x,y);
    fw_stencil_end();
}

static void renderLogoPath(enum ImagesEnum imageEnum, int x, int y, fx_path *paths, int numPaths, fw_vec4f *percentages) {
    fw_stencil_begin();
    {
        fw_stencil_activateStencilBuffer();
        fw_vec3f offsetVector = {x,y};

        for (int i=0; i<numPaths; i++) {
            fx_path_render(paths[i].vertices, paths[i].numVertices, percentages[i].x, &offsetVector);
        }

        fw_stencil_activateColorBuffer(0);
        renderLogo(imageEnum, x,y);
    }
    fw_stencil_end();
}

static void renderStarTrail(fw_timer_data *time) {
    if (_starTrailEmitter.isParticleSpawnEnabled) {
        fx_knit_projection_perspective();

        _starTrailEmitter.pos.x = _starPos.x + _starPosOffset.x*sinf(1.6+2*time->elapsed);
        _starTrailEmitter.pos.y = _starPos.y + _starPosOffset.y*cosf(1.6+2*time->elapsed);
        _starTrailEmitter.pos.z = _starPos.z;

        fx_particles_update(&_starTrailEmitter, time);
        fx_particles_render(&_starTrailEmitter);
        fx_knit_projection_ortho();
    }
}

static void renderStar(fw_timer_data *time) {
    fx_knit_projection_perspective();

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW); // In fx_knit_projection_perspective Y gets scaled by -1.

    fx_knit_projection_perspective();

    glPushMatrix();
    {
        glTranslatef(
            _starPos.x + _starPosOffset.x*sinf(1.6+2*time->elapsed),
            _starPos.y + _starPosOffset.y*cosf(1.6+2*time->elapsed),
            _starPos.z
        );
        glRotatef(_starRot.w, _starRot.x, _starRot.y, _starRot.z);
        glScalef(3,3,3);
        fw_object_3d_put(&_star);
    }
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);

    fx_knit_projection_ortho();
}

static void renderPlainScene(fw_timer_data *time) {
    fx_knit_viewport(0,0);
    fx_knit_projection_ortho();

    renderBG1();
    renderFrame1();
    renderFramePattern1();
    renderDeer(TRUE, COLOR_3UB_WHITE_WITH_RED_TINT);
    renderTree(RES_IMG_TREE_MONO,
               _blackWhitePalette[(int)_fadePalettePos.x].x,
               _blackWhitePalette[(int)_fadePalettePos.x].y,
               _blackWhitePalette[(int)_fadePalettePos.x].z);
    renderTreeStars();

    if (isRunning(ANIM_LOGO_SPECTROX_PATH)) {
        renderLogoPath(RES_IMG_LOGO_SPECTROX, LOGO_SPECTROX_POS, _spectrox_paths, SPECTROX_PATH_ENUM_SIZE, _logoSpectroxPathPercentages);
    }
    if (isRunning(ANIM_LOGO_SPECTROX_BLINDS)) {
        renderLogoBlinds(RES_IMG_LOGO_SPECTROX, LOGO_SPECTROX_POS, 12, _logoSpectroxBlindsHeight1.x, _logoSpectroxBlindsHeight2.x);
    }
    if (isRunning(ANIM_LOGO_DESIRE_PATH)) {
        renderLogoPath(RES_IMG_LOGO_DESIRE, LOGO_DESIRE_POS, _desire_paths, DESIRE_PATH_ENUM_SIZE, _logoDesirePathPercentages);
    }
    if (isRunning(ANIM_LOGO_DESIRE_BLINDS)) {
        renderLogoBlinds(RES_IMG_LOGO_DESIRE, LOGO_DESIRE_POS, 12, _logoDesireBlindsHeight1.x, _logoDesireBlindsHeight2.x);
    }
    if (isRunning(ANIM_STAR_IN)) {
        renderStarTrail(time);
        renderStar(time);
    }
}

// +++++++++++++++++++ //
// SCENE: MAIN
// +++++++++++++++++++ //

static void renderCanvasTransition(fw_timer_data *time) {
    glColor4ub(_snowTransitionColor.x,_snowTransitionColor.y,_snowTransitionColor.z,_snowTransitionColor.w);
    fw_quad_put(0,0,FX_KNIT_CANVAS_WIDTH, FX_KNIT_CANVAS_HEIGHT);
}

static void subtractLogoStencil(float angle) {
    const float w = 100;
    const float h = 200;
    glColor3ub(0,0,0);
    glTranslatef(FX_KNIT_CANVAS_WIDTH*.5f,-FX_KNIT_CANVAS_HEIGHT,0);
    glTranslatef(0,h*.72,0);
    glRotatef(angle, 0,0,1);
    glTranslatef(0,-h*.72,0);
    fw_quad_put(0,0,w,h);
}

static void subtractLogoDesireStencil(fw_timer_data *time) {
    subtractLogoStencil(_logoDesireStencilRot.x);
}

static void subtractLogoSpectroxStencil(fw_timer_data *time) {
    subtractLogoStencil(_logoSpectroxStencilRot.x);
}

static void renderStenciledLogo(enum ImagesEnum imagesEnum, int x, int y, void (*stencil_subtract_proc)(fw_timer_data*), fw_timer_data *time) {

    fw_stencil_begin();
    {
        fw_stencil_activateStencilBuffer();

        // Shadow stencil has ref id 2.
        glStencilFunc(GL_ALWAYS, 2, 0xFF);
        fx_knit_imageBegin(&getImages()[imagesEnum]);
        glColor3ub(0,0,0);
        glTranslatef(x+1,y+1,0);
        fw_image_putImage(&getImages()[imagesEnum]);
        fx_knit_imageEnd();

        // Logo stencil has ref id 3.
        glStencilFunc(GL_ALWAYS, 3, 0xFF);
        fx_knit_imageBegin(&getImages()[imagesEnum]);
        glColor3ub(0,0,0);
        glTranslatef(x,y,0);
        fw_image_putImage(&getImages()[imagesEnum]);
        fx_knit_imageEnd();

        // Hack: Draw additional candle flame for Desire logo on stencil ref 4.
        if (imagesEnum == RES_IMG_LOGO_DESIRE) {
            glStencilFunc(GL_ALWAYS, 4, 0xFF);
            fx_ornaments_renderDesireCandle();
        }

        // Subtract from stencil.
        glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
        glPushMatrix();

        if (stencil_subtract_proc != NULL) {
            stencil_subtract_proc(time);
        }
        glPopMatrix();

        fw_stencil_activateColorBuffer(0);

        // Fill shadow in stencil ref id 2.
        glStencilFunc(GL_EQUAL, 2, 0xFF);
        glColor4ub(COLOR_4UB_SHADOW);
        fw_quad_put(0,0,FX_KNIT_CANVAS_WIDTH,FX_KNIT_CANVAS_WIDTH);

        // Fill logo gradient in stencil ref id 3.
        glStencilFunc(GL_EQUAL, 3, 0xFF);
        fx_knit_imageBegin(&getImages()[RES_IMG_LOGO_GRADIENT]);
        fw_image_putImageExt(&getImages()[RES_IMG_LOGO_GRADIENT], 100,0, 100, -time->elapsed * 2);
        fx_knit_imageEnd();

        // Hack: Fill logo gradient in stencil ref id 4 (Desire candle flame).
        if (imagesEnum == RES_IMG_LOGO_DESIRE) {
            glStencilFunc(GL_EQUAL, 4, 0xFF);
            fx_knit_imageBegin(&getImages()[RES_IMG_FLAME_GRADIENT]);
            fw_image_putImageExt(&getImages()[RES_IMG_FLAME_GRADIENT], 100,0, 100, time->elapsed * 2);
            fx_knit_imageEnd();
        }

    }
    fw_stencil_end();
}

static void renderDeerShadows(void) {

    // Left deer.
    fx_knit_imageBegin(&getImages()[_syncDeerLeftImagesEnum]);
    glColor4ub(COLOR_4UB_SHADOW);
    glTranslatef(16-_deerPosDelta.x,49,0);
    fw_image_putImage(&getImages()[_syncDeerLeftImagesEnum]);
    fx_knit_imageEnd();

    // Right deer.
    fx_knit_imageBegin(&getImages()[_syncDeerRightImagesEnum]);
    glColor4ub(COLOR_4UB_SHADOW);
    glTranslatef(83+_deerPosDelta.x,49,0);
    glScalef(-1,1,1);
    fw_image_putImage(&getImages()[_syncDeerRightImagesEnum]);
    fx_knit_imageEnd();
}

static void renderTreeShadow(enum ImagesEnum imagesEnum) {
    fx_knit_imageBegin(&getImages()[imagesEnum]);
    glColor4ub(COLOR_4UB_SHADOW);
    glTranslatef(36,41,0);
    fw_image_putImage(&getImages()[imagesEnum]);
    fx_knit_imageEnd();
}

static void renderTreeStarsWithGradient(fw_timer_data *time) {
    fw_stencil_begin();
    fw_stencil_activateStencilBuffer();

    renderTreeStars();

    fw_stencil_activateColorBuffer(0);

    fx_knit_imageBegin(&getImages()[RES_IMG_LOGO_GRADIENT]);
    fw_image_putImageExt(&getImages()[RES_IMG_LOGO_GRADIENT], 100,0, 100, -time->elapsed * 2);
    fx_knit_imageEnd();

    fw_stencil_end();
}

static void renderScroller(const char string[], int xoff, int yoff, int r, int g, int b, int a, fw_timer_data *time) {
    fw_font_renderTextBegin(getFontFace());
    {
        fx_knit_projection_ortho();

        glPushMatrix();
        glTranslatef(_scrollerPos.x, 73,0);
        glColor4ub(r,g,b,a);
        fw_font_renderSingleTextLine(string, getFontFace(), xoff,yoff);
        glPopMatrix();
    }
    fw_font_renderTextEnd();
}

#define SCROLLER_STENCIL_WITH 6
static void renderScrollerStenciled(fw_timer_data *time) {
    fw_stencil_begin();
    fw_stencil_activateStencilBuffer();

    glPushMatrix();
    glColor3ub(0,200,0);
    glTranslatef(0,72,0);
    fw_quad_put(0,0,FX_KNIT_CANVAS_WIDTH-SCROLLER_STENCIL_WITH, 13);
    glTranslatef(FX_KNIT_CANVAS_WIDTH-SCROLLER_STENCIL_WITH, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2i(0,0);
    glVertex2i(0,13);
    glVertex2i(SCROLLER_STENCIL_WITH,0);
    glEnd();
    glPopMatrix();

    fw_stencil_activateColorBuffer(0);

    renderScroller(_scrollText, 0,1,
                   _blackWhitePalette[(int)_fadePalettePos.x].x,
                   _blackWhitePalette[(int)_fadePalettePos.x].y,
                   _blackWhitePalette[(int)_fadePalettePos.x].z,
                   255, time);

    fw_stencil_end();
}

static void renderSanta() {
    fx_knit_imageBegin(&getImages()[_syncSantaImagesEnum]);

    glColor4ub(COLOR_4UB_SHADOW);

    glPushMatrix();
    glTranslatef(_santaPosX.x-1,_santaPosY.x,0);
    fw_image_putImage(&getImages()[_syncSantaImagesEnum]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(_santaPosX.x+1,_santaPosY.x,0);
    fw_image_putImage(&getImages()[_syncSantaImagesEnum]);
    glPopMatrix();

    glColor4ub(255,255,255,255);

    glTranslatef(_santaPosX.x,_santaPosY.x,0);
    fw_image_putImage(&getImages()[_syncSantaImagesEnum]);
    fx_knit_imageEnd();
}

static void renderCanvasWithShadows(fw_timer_data *time) {
    fx_knit_viewport(0,0);
    fx_knit_projection_ortho();

    renderBG1();

    if (_auroraAlpha.x > 0) {
        fx_aurora_render(_auroraAlpha.x, time);
    }

    renderFrame1();

    if (_twisterStencilPos.x != _twisterStencilPosInit.x
            && _twisterStencilPos.x != _twisterStencilPosFinal.x) {
        fx_twister_render(_twisterStencilPos.x,time);
        fx_knit_projection_ortho();
    }

    if (_logoSpectroxStencilRot.x != 270 && _logoSpectroxStencilRot.x != -90) {
        renderStenciledLogo(RES_IMG_LOGO_SPECTROX, LOGO_SPECTROX_POS, subtractLogoSpectroxStencil, time);
    }

    if (_logoDesireStencilRot.x != 270 && _logoDesireStencilRot.x != -90) {
        renderStenciledLogo(RES_IMG_LOGO_DESIRE, LOGO_DESIRE_POS, subtractLogoDesireStencil, time);
    }

    renderDeerShadows();
    renderDeer(FALSE, 255,255,255);
    renderTreeShadow(RES_IMG_TREE);
    renderTree(RES_IMG_TREE, 255,255,255);

    renderTreeStarsWithGradient(time);

    fx_ornaments_updateCandles(_candles, _candlesLength, time);
    fx_ornaments_renderCandles(_candles, _candlesLength);

    if (_starsPercentage.x > 0) {
        fx_ornaments_updateStars(_stars, _starsLength, _starsPercentage.x, time);
        fx_ornaments_renderStars(_stars, _starsLength);
    }
}

static const fw_vec2f _rotationCenterPoint = {49,60};
static fw_vec2f _orbitPoint = {50};
static fw_vec2f _resultPoint = {0};

static void renderRotation(fw_timer_data *time) {
    _orbitPoint.y = 20 + fw_math_randf(-7,7);

    fw_math_rotate(&_orbitPoint, &_rotationCenterPoint, _snowEmitterRot.x, &_resultPoint);

    _snowEmitter.pos.x = _resultPoint.x;
    _snowEmitter.pos.y = _resultPoint.y;
    _snowEmitter.pos.z = 0;
    fx_particles_update(&_snowEmitter, time);
    fx_particles_render(&_snowEmitter);
}

static void renderMainScene(fw_timer_data *time) {
    renderCanvasWithShadows(time);

    if (_snowTransitionColor.w != _snowTransitionColorFinal.w) {
        renderCanvasTransition(time);
    }

    renderScrollerStenciled(time);

    if (_santaPosX.x >= -getImages()[RES_IMG_SANTA_WALK_1].width
            && _santaPosX.x < FX_KNIT_CANVAS_WIDTH) {
        renderSanta();
    }

    renderRotation(time);
}

#if FW_SCANLINES_ENABLED
static void renderScanlines(void) {
    fw_render_viewportHiRes(0,0);
    fw_projection_ortho();
    glColor4f(0,0,0,FW_SCANLINES_ALPHA);
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_LINES);
    for (float i=0; i<FW_WINDOW_HEIGHT/2; i+=1) {
        glVertex2f(0, i);
        glVertex2f(FW_WINDOW_WIDTH, i);
    }
    glEnd();
    glPopMatrix();
}
#endif

static void handleSnowUpdate(fw_timer_data *time) {
    for (int i=0; i<_snowDataLength; i++) {
        snow_data *s = &_snowData[i];

        if (s->isRespawnEnabledCurrent != s->isRespawnEnabledTarget.x) {
            s->isRespawnEnabledCurrent = s->isRespawnEnabledTarget.x;

            if (s->isRespawnEnabledCurrent) {
                fx_snow_init(s->flakes, s->numFlakes, s->smallFlakesPercentage);
            }
        }

        if (s->isEnabled.x) {
            fx_snow_update(s->flakes, s->numFlakes, s->isRespawnEnabledCurrent, time);
        }
    }
}

void scene_0_execute(fw_timer_data *time) {
    fw_sync_process(&_syncData, time);
    fw_animation_processAnimationSchedule(&_animSchedule, time);

    handleSnowUpdate(time);

    // Scene time.
    fw_timer_data at;


    // Low-res.
    /*
    fw_render_bindTextureBufferBegin();
    {
        // fw_render_viewportLowRes(0,0);

        // ...

    }
    fw_render_bindTextureBufferEnd();
    */

    // High-res.
    fw_render_viewportHiRes(0,0);
    {
        fw_render_clearHiRes(0,0,0,1);

        if (isRunning(ANIM_START)) {
            at = animTime(ANIM_START, time);
            renderStartScene(&at);
        }

        if (isRunning(ANIM_LOGO_SPECTROX_PATH) ||
                isRunning(ANIM_LOGO_SPECTROX_BLINDS) ||
                isRunning(ANIM_LOGO_DESIRE_PATH) ||
                isRunning(ANIM_LOGO_DESIRE_BLINDS) ||
                isRunning(ANIM_STAR_IN)
           ) {
            at = animTime(ANIM_LOGO_SPECTROX_PATH, time);
            renderPlainScene(&at);
        }

        if (isRunning(ANIM_MAIN)) {
            at = animTime(ANIM_MAIN, time);
            renderMainScene(&at);
        }

        renderSnowData();
        fx_knit_readPixels();
        fx_knit_render(time);
    }

    // Render low-res content from texture.
    //fw_render_renderTextureBuffer();

    // Render additional hi-res content.
    //fw_render_viewportHiRes(0,0);

    // ...

#if FW_SCANLINES_ENABLED
    renderScanlines();
#endif

}
