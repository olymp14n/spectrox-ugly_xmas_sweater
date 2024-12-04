#include "path.h"
#include <GL/gl.h>
#include "../fw/math.h"

static void drawLine(fw_vec3f *v1, fw_vec3f *v2, fw_vec3f *offsetVec) {
    glColor3ub(255,255,0);
    glLineWidth(3);
    glPushMatrix();
    glTranslatef(offsetVec->x, offsetVec->y, offsetVec->z);
    glTranslatef(.5, .5, 0); // GL_LINES pixel precision hack: https://community.khronos.org/t/glortho-coordinates/54227/4
    glBegin(GL_LINES);
        glVertex3f(v1->x, v1->y, v1->z);
        glVertex3f(v2->x, v2->y, v2->z);
    glEnd();
    glPopMatrix();
}

void fx_path_render(fw_vec3f *verts, int vertsLen, float p, fw_vec3f *offsetVec) {
    if (vertsLen < 2 || p <= 0) {
        return;
    }

    if (p >= 1) {
        for (int i=0; i<vertsLen-1; i++) {
            drawLine(&verts[i], &verts[i+1], offsetVec);
        }
        return;
    }

    float totalLength = 0;
    for (int i=0; i<vertsLen-1; i++) {
        totalLength += fw_math_dist(&verts[i], &verts[i+1]);
    }

    float targetLength = p * totalLength;

    float accumulatedLength = 0;
    for (int i=0; i<vertsLen-1; i++) {
        float segmentLength = fw_math_dist(&verts[i], &verts[i+1]);
        if (accumulatedLength + segmentLength >= targetLength) {
            float remainingLength = targetLength - accumulatedLength;
            float t = remainingLength / segmentLength;

            fw_vec3f targetVec;
            targetVec.x = fw_math_lerp(verts[i].x, verts[i+1].x, t);
            targetVec.y = fw_math_lerp(verts[i].y, verts[i+1].y, t);
            targetVec.z = fw_math_lerp(verts[i].z, verts[i+1].z, t);
            drawLine(&verts[i], &targetVec, offsetVec);
            break;

        } else {
            drawLine(&verts[i], &verts[i+1], offsetVec);
            accumulatedLength += segmentLength;
        }
    }
}
