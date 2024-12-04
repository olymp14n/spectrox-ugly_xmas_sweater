
typedef enum {
    DESIRE_PATH_D1_1,
    DESIRE_PATH_D1_2,
    DESIRE_PATH_E1_1,
    DESIRE_PATH_E1_2,
    DESIRE_PATH_E1_3,
    DESIRE_PATH_E1_4,
    DESIRE_PATH_S1_1,
    DESIRE_PATH_I1_1,
    DESIRE_PATH_I1_2,
    DESIRE_PATH_R1_1,
    DESIRE_PATH_R1_2,
    DESIRE_PATH_E2_1,
    DESIRE_PATH_E2_2,
    DESIRE_PATH_E2_3,
    DESIRE_PATH_E2_4,

    DESIRE_PATH_ENUM_SIZE
} desire_path_enum;

static fw_vec3f _desire_path_d1_1[] = {
    {11,6,0},
    {11,25,0},
};

static fw_vec3f _desire_path_d1_2[] = {
    {5,11,0},
    {3,9,0},
    {5,7,0},
    {7,8,0},
    {8,14,0},
    {2,14,0},
    {0,8,0},
    {3,4,0},
    {13,4,0},
    {19,9,0},
    {21,15,0},
    {20,25,0},
    {16,28,0},
    {9,25,0},
    {5,26,0},
    {0,29,0},
    {1,34,0},
    {5,35,0},
    {7,32,0},
    {6,29,0},
    {3,30,0},
    {3,32,0},
    {6,32,0},
};

static fw_vec3f _desire_path_e1_1[] = {
    {25, 8, 0},
    {25, 18, 0},
};

static fw_vec3f _desire_path_e1_2[] = {
    {23, 6, 0},
    {30, 6, 0},
    {32, 9, 0},
};

static fw_vec3f _desire_path_e1_3[] = {
    {25, 12, 0},
    {30, 12, 0},
};

static fw_vec3f _desire_path_e1_4[] = {
    {23, 18, 0},
    {30, 18, 0},
    {31, 14, 0},
};

static fw_vec3f _desire_path_s1_1[] = {
    {39, 6, 0},
    {39, 4, 0},
    {34, 5, 0},
    {36, 10, 0},
    {40, 12, 0},
    {39, 16, 0},
    {34, 15, 0},
    {33, 10, 0},
};

static fw_vec3f _desire_path_i1_1[] = {
    {42, 5, 0},
    {47, 5, 0},
    {44, 5, 0},
    {44, 17, 0},
    {42, 17, 0},
    {48, 17, 0},
};

static fw_vec3f _desire_path_i1_2[] = {
    {45, 0, 0},
    {45, 4, 0},
};

static fw_vec3f _desire_path_r1_1[] = {
    {52, 8, 0},
    {52, 18, 0},
    {50, 18, 0},
    {56, 18, 0},
};

static fw_vec3f _desire_path_r1_2[] = {
    {49, 6, 0},
    {57, 6, 0},
    {58, 8, 0},
    {55, 12, 0},
    {57, 17, 0},
    {59, 23, 0},
    {62, 28, 0},
    {66, 31, 0},
    {70, 33, 0},
    {75, 31, 0},
    {77, 28, 0},
    {77, 22, 0},
    {74, 20, 0},
    {70, 22, 0},
    {71, 27, 0},
    {75, 26, 0},
    {75, 24, 0},
    {73, 24, 0},
};

static fw_vec3f _desire_path_e2_1[] = {
    {63, 9, 0},
    {63, 19, 0},
};

static fw_vec3f _desire_path_e2_2[] = {
    {61, 7, 0},
    {68, 7, 0},
    {70, 10, 0},
};

static fw_vec3f _desire_path_e2_3[] = {
    {63, 13, 0},
    {68, 13, 0},
};

static fw_vec3f _desire_path_e2_4[] = {
    {61, 19, 0},
    {68, 19, 0},
    {69, 15, 0},
};

fx_path _desire_paths[] = {
    [DESIRE_PATH_D1_1] = {
        .vertices = _desire_path_d1_1,
        .numVertices = LEN(_desire_path_d1_1),
    },
    [DESIRE_PATH_D1_2] = {
        .vertices = _desire_path_d1_2,
        .numVertices = LEN(_desire_path_d1_2),
    },
    [DESIRE_PATH_E1_1] = {
        .vertices = _desire_path_e1_1,
        .numVertices = LEN(_desire_path_e1_1),
    },
    [DESIRE_PATH_E1_2] = {
        .vertices = _desire_path_e1_2,
        .numVertices = LEN(_desire_path_e1_2),
    },
    [DESIRE_PATH_E1_3] = {
        .vertices = _desire_path_e1_3,
        .numVertices = LEN(_desire_path_e1_3),
    },
    [DESIRE_PATH_E1_4] = {
        .vertices = _desire_path_e1_4,
        .numVertices = LEN(_desire_path_e1_4),
    },
    [DESIRE_PATH_S1_1] = {
        .vertices = _desire_path_s1_1,
        .numVertices = LEN(_desire_path_s1_1),
    },
    [DESIRE_PATH_I1_1] = {
        .vertices = _desire_path_i1_1,
        .numVertices = LEN(_desire_path_i1_1),
    },
    [DESIRE_PATH_I1_2] = {
        .vertices = _desire_path_i1_2,
        .numVertices = LEN(_desire_path_i1_2),
    },
    [DESIRE_PATH_R1_1] = {
        .vertices = _desire_path_r1_1,
        .numVertices = LEN(_desire_path_r1_1),
    },
    [DESIRE_PATH_R1_2] = {
        .vertices = _desire_path_r1_2,
        .numVertices = LEN(_desire_path_r1_2),
    },
    [DESIRE_PATH_E2_1] = {
        .vertices = _desire_path_e2_1,
        .numVertices = LEN(_desire_path_e2_1),
    },
    [DESIRE_PATH_E2_2] = {
        .vertices = _desire_path_e2_2,
        .numVertices = LEN(_desire_path_e2_2),
    },
    [DESIRE_PATH_E2_3] = {
        .vertices = _desire_path_e2_3,
        .numVertices = LEN(_desire_path_e2_3),
    },
    [DESIRE_PATH_E2_4] = {
        .vertices = _desire_path_e2_4,
        .numVertices = LEN(_desire_path_e2_4),
    },

};
