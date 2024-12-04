
typedef enum {
    SPECTROX_PATH_S1,
    SPECTROX_PATH_P1,
    SPECTROX_PATH_E1,
    SPECTROX_PATH_E2,
    SPECTROX_PATH_E3,
    SPECTROX_PATH_E4,
    SPECTROX_PATH_C1,
    SPECTROX_PATH_T1,
    SPECTROX_PATH_T2,
    SPECTROX_PATH_R1,
    SPECTROX_PATH_O1,
    SPECTROX_PATH_X1,
    SPECTROX_PATH_X2,

    SPECTROX_PATH_ENUM_SIZE
} spectrox_path_enum;

static fw_vec3f _spectrox_path_s1[] = {
    {12,12,0},
    {14,10,0},
    {11,8,0},
    {8,10,0},
    {8,12,0},
    {16,22,0},
    {14,31,0},
    {9,34,0},
    {3,33,0},
    {0,27,0},
    {2,23,0},
    {7,24,0},
    {7,29,0},
    {3,28,0},
    {4,25,0},
};

static fw_vec3f _spectrox_path_p1[] = {
    {23,19,0},
    {25,14,0},
    {25,11,0},
    {20,11,0},
    {20,23,0},
    {18,23,0},
    {23,23,0},
};

static fw_vec3f _spectrox_path_e1[] = {
    {29,10,0},
    {29,20,0},
};

static fw_vec3f _spectrox_path_e2[] = {
    {27,8,0},
    {34,8,0},
    {36,11,0},
};

static fw_vec3f _spectrox_path_e3[] = {
    {29,14,0},
    {34,14,0},
};

static fw_vec3f _spectrox_path_e4[] = {
    {27,20,0},
    {34,20,0},
    {35,16,0},
};

static fw_vec3f _spectrox_path_c1[] = {
    {41,13,0},
    {44,12,0},
    {43,9,0},
    {37,10,0},
    {37,19,0},
    {39,19,0},
    {39,21,0},
    {43,21,0},
    {44,17,0},
};

static fw_vec3f _spectrox_path_t1[] = {
    {31,6,0},
    {33,3,0},
    {40,2,0},
    {47,5,0},
    {54,6,0},
    {59,5,0},
    {62,1,0},
    {56,0,0},
};

static fw_vec3f _spectrox_path_t2[] = {
    {48,7,0},
    {47,21,0},
    {45,21,0},
    {50,21,0},
};

static fw_vec3f _spectrox_path_r1[] = {
    {51,20,0},
    {55,20,0},
    {53,20,0},
    {53,8,0},
    {51,8,0},
    {57,8,0},
    {59,11,0},
    {55,14,0},
    {58,18,0},
    {59,23,0},
    {64,31,0},
    {71,30,0},
    {72,23,0},
    {67,22,0},
    {65,26,0},
    {67,28,0},
    {69,27,0},
    {67,24,0},
};

static fw_vec3f _spectrox_path_o1[] = {
    {66,11,0},
    {62,12,0},
    {60,16,0},
    {63,20,0},
    {68,19,0},
    {70,15,0},
    {69,12,0},
    {66,11,0},
};

static fw_vec3f _spectrox_path_x1[] = {
    {76,10,0},
    {77,12,0},
    {69,20,0},
    {74,20,0},
};

static fw_vec3f _spectrox_path_x2[] = {
    {73,11,0},
    {74,13,0},
    {74,16,0},
    {77,24,0},
    {81,25,0},
    {82,22,0},
    {81,19,0},
    {79,19,0},
    {79,21,0},
    {81,23,0},
};

fx_path _spectrox_paths[] = {
    [SPECTROX_PATH_S1] = {
        .vertices = _spectrox_path_s1,
        .numVertices = LEN(_spectrox_path_s1),
    },
    [SPECTROX_PATH_P1] = {
        .vertices = _spectrox_path_p1,
        .numVertices = LEN(_spectrox_path_p1),
    },
    [SPECTROX_PATH_E1] = {
        .vertices = _spectrox_path_e1,
        .numVertices = LEN(_spectrox_path_e1),
    },
    [SPECTROX_PATH_E2] = {
        .vertices = _spectrox_path_e2,
        .numVertices = LEN(_spectrox_path_e2),
    },
    [SPECTROX_PATH_E3] = {
        .vertices = _spectrox_path_e3,
        .numVertices = LEN(_spectrox_path_e3),
    },
    [SPECTROX_PATH_E4] = {
        .vertices = _spectrox_path_e4,
        .numVertices = LEN(_spectrox_path_e4),
    },
    [SPECTROX_PATH_C1] = {
        .vertices = _spectrox_path_c1,
        .numVertices = LEN(_spectrox_path_c1),
    },
    [SPECTROX_PATH_T1] = {
        .vertices = _spectrox_path_t1,
        .numVertices = LEN(_spectrox_path_t1),
    },
    [SPECTROX_PATH_T2] = {
        .vertices = _spectrox_path_t2,
        .numVertices = LEN(_spectrox_path_t2),
    },
    [SPECTROX_PATH_R1] = {
        .vertices = _spectrox_path_r1,
        .numVertices = LEN(_spectrox_path_r1),
    },
    [SPECTROX_PATH_O1] = {
        .vertices = _spectrox_path_o1,
        .numVertices = LEN(_spectrox_path_o1),
    },
    [SPECTROX_PATH_X1] = {
        .vertices = _spectrox_path_x1,
        .numVertices = LEN(_spectrox_path_x1),
    },
    [SPECTROX_PATH_X2] = {
        .vertices = _spectrox_path_x2,
        .numVertices = LEN(_spectrox_path_x2),
    },
};
