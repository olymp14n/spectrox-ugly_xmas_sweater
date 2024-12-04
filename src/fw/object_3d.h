#ifndef FW_OBJECT_3D_H
#define FW_OBJECT_3D_H

#include "types.h"

typedef enum {
    MAT_TYPE_NONE,
    MAT_TYPE_EYE_LINEAR,

    MATERIAL_TYPE_ENUM_SIZE
} MaterialTypeEnum;

typedef struct {
    MaterialTypeEnum matType;
    unsigned int textureID;
    float scale;
} fw_object_3d_material;

typedef struct {
    int numFaces; // Number of faces.
    int *f;       // Every face points to 3 unique vertices.
    float *v;     // Vertices, each made of 3 coordinates.
    float *n;     // Vertex normals, each made of 3 coordinates.
    float *c;     // Vertex colors, each made of 4 values.
    float *t;     // Texture coordinates, each made of 2 values.
} fw_object_3d_array;

typedef struct {
    fw_vec3f *v;
    int numVertices;
    fw_vec3i *f;
    int numFaces;
    fw_vec4f *c;
    int numC;
    int *cidx;
    fw_vec3f *fn;
    fw_vec2f *t;
    fw_object_3d_material *m;
    fw_object_3d_array arr;
} fw_object_3d;

void fw_object_3d_putFaces(fw_object_3d *object3D);
void fw_object_3d_putFacesArray(fw_object_3d *object3D);
void fw_object_3d_put(fw_object_3d *object3D);
void fw_object_3d_calculateFaceNormals(fw_object_3d *object3D);

void fw_object_3d_initArray(fw_object_3d *object3D);
void fw_object_3d_updateArray(fw_object_3d *object3D);


#endif
