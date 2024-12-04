fw_font_glyph _fontGlyphs[] = {
    [32] = {50, 36, 3, 11, -1, 0, 3},
    [33] = {62, 36, 1, 11, 0, 0, 2},
    [34] = {60, 12, 3, 11, 0, 0, 4},
    [37] = {18, 0, 7, 11, 0, 0, 8},
    [39] = {6, 48, 1, 11, 0, 0, 2},
    [40] = {0, 48, 2, 11, 0, 0, 3},
    [41] = {3, 48, 2, 11, 0, 0, 3},
    [43] = {12, 12, 5, 11, 0, 0, 6},
    [44] = {8, 48, 1, 11, 0, 0, 2},
    [45] = {24, 12, 5, 11, 0, 0, 6},
    [46] = {10, 48, 1, 11, 0, 0, 2},
    [47] = {60, 24, 3, 11, 0, 0, 4},
    [48] = {40, 0, 5, 11, 0, 0, 6},
    [49] = {54, 36, 3, 11, 0, 0, 4},
    [50] = {48, 12, 5, 11, 0, 0, 6},
    [51] = {54, 12, 5, 11, 0, 0, 6},
    [52] = {0, 24, 5, 11, 0, 0, 6},
    [53] = {6, 24, 5, 11, 0, 0, 6},
    [54] = {12, 24, 5, 11, 0, 0, 6},
    [55] = {18, 24, 5, 11, 0, 0, 6},
    [56] = {24, 24, 5, 11, 0, 0, 6},
    [57] = {30, 24, 5, 11, 0, 0, 6},
    [58] = {14, 48, 1, 11, 0, 0, 2},
    [59] = {12, 48, 1, 11, 0, 0, 2},
    [63] = {40, 36, 4, 11, 0, 0, 5},
    [65] = {6, 12, 5, 11, 0, 0, 6},
    [66] = {42, 24, 5, 11, 0, 0, 6},
    [67] = {48, 24, 5, 11, 0, 0, 6},
    [68] = {54, 24, 5, 11, 0, 0, 6},
    [69] = {30, 36, 4, 11, 0, 0, 5},
    [70] = {0, 36, 5, 11, 0, 0, 6},
    [71] = {6, 36, 5, 11, 0, 0, 6},
    [72] = {12, 36, 5, 11, 0, 0, 6},
    [73] = {58, 36, 3, 11, 0, 0, 4},
    [74] = {45, 36, 4, 11, 0, 0, 5},
    [75] = {18, 36, 5, 11, 0, 0, 6},
    [76] = {35, 36, 4, 11, 0, 0, 5},
    [77] = {10, 0, 7, 11, 0, 0, 8},
    [78] = {26, 0, 6, 11, 0, 0, 7},
    [79] = {24, 36, 5, 11, 0, 0, 6},
    [80] = {46, 0, 5, 11, 0, 0, 6},
    [81] = {33, 0, 6, 11, 0, 0, 7},
    [82] = {52, 0, 5, 11, 0, 0, 6},
    [83] = {58, 0, 5, 11, 0, 0, 6},
    [84] = {36, 24, 5, 11, 0, 0, 6},
    [85] = {0, 12, 5, 11, 0, 0, 6},
    [86] = {18, 12, 5, 11, 0, 0, 6},
    [87] = {0, 0, 9, 11, 0, 0, 10},
    [88] = {30, 12, 5, 11, 0, 0, 6},
    [89] = {36, 12, 5, 11, 0, 0, 6},
    [90] = {42, 12, 5, 11, 0, 0, 6},
};

fw_font_face _fontFace = {
    .outline = 0,
    .lineHeight = 11,
    .textureWidth = 64,
    .textureHeight = 64,
    .glyphs = _fontGlyphs,
    .numGlyphs = (sizeof(_fontGlyphs) / sizeof(_fontGlyphs[0])),
};
