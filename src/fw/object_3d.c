#include "object_3d.h"
#include <GL/gl.h>
#include "texgen.h"

void fw_object_3d_putFaces(fw_object_3d *object3D) {
    fw_vec3f *v = object3D->v;
    fw_vec3i *f = object3D->f;
    fw_vec4f *c = object3D->c;
    int *cidx = object3D->cidx;
    fw_vec3f *fn = object3D->fn;
    fw_vec2f *t = object3D->t;

    // Iterate over each color.
    for (int j=0; j<object3D->numC; j++) {

        glColor4f(c[j].x, c[j].y, c[j].z, c[j].w);

        const int isMatTypeEyeLinear = object3D->m != NULL && object3D->m[j].matType == MAT_TYPE_EYE_LINEAR;
        if (isMatTypeEyeLinear) {
            glBindTexture(GL_TEXTURE_2D, object3D->m[j].textureID);
            fx_texgen_eye_linear_begin(object3D->m[j].scale);
        }

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < object3D->numFaces; i++) {
            // Skip face if not of the current color.
            if (cidx[i] != j) {
                continue;
            }

            glNormal3f(fn[i].x, fn[i].y, fn[i].z);

            if (t != NULL) {
                glTexCoord2f(t[f[i].x].x, t[f[i].x].y);
            }
            glVertex3f(v[f[i].x].x, v[f[i].x].y, v[f[i].x].z);

            if (t != NULL) {
                glTexCoord2f(t[f[i].y].x, t[f[i].y].y);
            }
            glVertex3f(v[f[i].y].x, v[f[i].y].y, v[f[i].y].z);

            if (t != NULL) {
                glTexCoord2f(t[f[i].z].x, t[f[i].z].y);
            }
            glVertex3f(v[f[i].z].x, v[f[i].z].y, v[f[i].z].z);
        }
        glEnd();

        if (isMatTypeEyeLinear) {
            fx_texgen_eye_linear_end();
        }
    }
}

/**
 * Render arrays. Client states need to enabled before once:
 *  - glEnableClientState(GL_VERTEX_ARRAY);
 *  - glEnableClientState(GL_NORMAL_ARRAY);
 *  - glEnableClientState(GL_COLOR_ARRAY);
 *  - glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 */
void fw_object_3d_putFacesArray(fw_object_3d *object3D) {
    glVertexPointer(3,GL_FLOAT,0,object3D->arr.v);
    glNormalPointer(GL_FLOAT,0,object3D->arr.n);
    glColorPointer(4,GL_FLOAT,0,object3D->arr.c);

    if (object3D->t != NULL) {
        glTexCoordPointer(2, GL_FLOAT, 0, object3D->arr.t);
    }

    glDrawElements(GL_TRIANGLES, object3D->arr.numFaces, GL_UNSIGNED_INT, object3D->arr.f);
}

void fw_object_3d_put(fw_object_3d *object3D) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    fw_object_3d_putFaces(object3D);
    glDisable(GL_CULL_FACE);
}

static void fw_object_3d_cross(fw_vec3f *a, fw_vec3f *b, fw_vec3f *result) {
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

static void fw_object_3d_calculateNormal(fw_vec3f *a, fw_vec3f *b, fw_vec3f *c, fw_vec3f *result) {
    fw_vec3f v1 = {a->x - c->x, a->y - c->y, a->z - c->z};
    fw_vec3f v2 = {b->x - c->x, b->y - c->y, b->z - c->z};

    fw_object_3d_cross(&v1, &v2, result);
    // Only needed if GL_NORMALIZE is not enabled.
    //fw_object_3d_normalize(result);
}

void fw_object_3d_calculateFaceNormals(fw_object_3d *object3D) {
    fw_vec3f *v = object3D->v;
    fw_vec3i *f = object3D->f;
    fw_vec3f *fn = object3D->fn;

    for (int i=0; i<object3D->numFaces; i++) {
        fw_object_3d_calculateNormal(&v[f[i].x], &v[f[i].y], &v[f[i].z], &fn[i]);
    }
}

void fw_object_3d_initArray(fw_object_3d *object3D) {
    int numFaces = object3D->numFaces * 3;
    int numVerts = object3D->numFaces * 3 * 3;
    int numColors = object3D->numFaces * 3 * 4;


    object3D->arr.numFaces = numFaces;
    object3D->arr.v = calloc(numVerts, sizeof(float));
    object3D->arr.n = calloc(numVerts, sizeof(float));
    object3D->arr.c = calloc(numColors, sizeof(float));
    object3D->arr.f = calloc(numFaces, sizeof(int));

    if (object3D->t != NULL) {
        int numTexCoords = object3D->numFaces * 3 * 2;
        object3D->arr.t = calloc(numTexCoords, sizeof(float));
    }

    for (int i=0; i<numFaces; i++) {
        object3D->arr.f[i] = i;
    }

    fw_object_3d_updateArray(object3D);
}

void fw_object_3d_updateArray(fw_object_3d *object3D) {
    int voff = 0;
    int coff = 0;
    int noff = 0;
    int toff = 0;
    for (int i=0; i<object3D->numFaces; i++) {
        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].x ].x;
        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].x ].y;
        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].x ].z;

        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].y ].x;
        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].y ].y;
        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].y ].z;

        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].z ].x;
        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].z ].y;
        object3D->arr.v[voff++] = object3D->v[ object3D->f[i].z ].z;

        if (object3D->t != NULL) {
            object3D->arr.t[toff++] = object3D->t[ object3D->f[i].x ].x;
            object3D->arr.t[toff++] = object3D->t[ object3D->f[i].x ].y;

            object3D->arr.t[toff++] = object3D->t[ object3D->f[i].y ].x;
            object3D->arr.t[toff++] = object3D->t[ object3D->f[i].y ].y;

            object3D->arr.t[toff++] = object3D->t[ object3D->f[i].z ].x;
            object3D->arr.t[toff++] = object3D->t[ object3D->f[i].z ].y;
        }

        // Normals and color per face, i.e. three times the same values for each vertex.
        for (int j=0; j<3; j++) {
            object3D->arr.n[noff++] = object3D->fn[i].x;
            object3D->arr.n[noff++] = object3D->fn[i].y;
            object3D->arr.n[noff++] = object3D->fn[i].z;

            object3D->arr.c[coff++] = object3D->c[ object3D->cidx[i] ].x;
            object3D->arr.c[coff++] = object3D->c[ object3D->cidx[i] ].y;
            object3D->arr.c[coff++] = object3D->c[ object3D->cidx[i] ].z;
            object3D->arr.c[coff++] = object3D->c[ object3D->cidx[i] ].w;
        }
    }
}
