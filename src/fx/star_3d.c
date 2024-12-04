#include "star_3d.h"
#include <stdlib.h>
#include <string.h>

static fw_vec3f _v[] = {
    {0.000000,1.000000,0.000000},
    {-0.951057,0.309017,0.000000},
    {-0.587785,-0.809017,0.000000},
    {0.587785,-0.809017,0.000000},
    {0.951057,0.309017,0.000000},
    {-0.236625,0.325687,0.000000},
    {-0.382867,-0.124401,0.000000},
    {0.000000,-0.402571,0.000000},
    {0.382867,-0.124401,0.000000},
    {0.236625,0.325687,0.000000},
    {0.000000,-0.000000,0.200000},
    {0.000000,-0.000000,-0.200000},
};
static fw_vec3i _f[] = {
    {4,9,10},
    {1,6,10},
    {2,7,10},
    {3,8,10},
    {0,5,10},
    {7,2,11},
    {8,3,11},
    {5,0,11},
    {9,4,11},
    {6,1,11},
    {4,8,11},
    {8,4,10},
    {5,1,10},
    {9,0,10},
    {6,2,10},
    {7,3,10},
    {1,5,11},
    {0,9,11},
    {2,6,11},
    {3,7,11},
};
static fw_vec4f _c[] = {
    {255/255.f,226/255.f,223/255.f,1}, // 0: Material.001
    {255/255.f,226/255.f,223/255.f,1}, // 1: Material.002
};
static int _cidx[] = {
    0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,
};

void fx_star_3d_init(fw_object_3d *object3D) {
    object3D->v = calloc(LEN(_v), sizeof(_v[0]));
    memcpy(object3D->v, _v, sizeof(_v));
    object3D->numVertices = LEN(_v);

    object3D->f = calloc(LEN(_f), sizeof(_f[0]));
    memcpy(object3D->f, _f, sizeof(_f));
    object3D->numFaces = LEN(_f);

    object3D->c = calloc(LEN(_c), sizeof(_c[0]));
    memcpy(object3D->c, _c, sizeof(_c));
    object3D->numC = LEN(_c);

    object3D->m = calloc(LEN(_c), sizeof(fw_object_3d_material));

    object3D->cidx = calloc(LEN(_cidx), sizeof(_cidx[0]));
    memcpy(object3D->cidx, _cidx, sizeof(_cidx));

    object3D->fn = calloc(LEN(_f), sizeof(fw_vec3f));
    fw_object_3d_calculateFaceNormals(object3D);
}
