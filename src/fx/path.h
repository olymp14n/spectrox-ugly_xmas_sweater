#ifndef FX_PATH_H
#define FX_PATH_H

#include "../fw/types.h"

typedef struct {
    fw_vec3f *vertices;
    int numVertices;
} fx_path;

void fx_path_render(fw_vec3f *verts, int vertsLen, float p, fw_vec3f *offsetVec);

#endif
