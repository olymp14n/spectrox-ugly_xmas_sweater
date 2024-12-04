#include "particles.h"

#include <stdlib.h>
#include <GL/gl.h>
#include "../fw/math.h"

static void updateParticlePosDefaultProc(fx_particle *p, fx_particle_emitter *e, fw_timer_data *time) {
    p->pos.y -= p->speed*time->delta;
}

void fx_particles_init(fx_particle_emitter *emitter, int numParticles, float particleMaxAge, float particleSpawnFreq) {
    fx_particle *particles = calloc(numParticles, sizeof(fx_particle));
    emitter->particles = particles;
    emitter->numParticles = numParticles;
    emitter->particleMaxAge = particleMaxAge;
    emitter->particleSpawnFreq= particleSpawnFreq;
    emitter->particleSpawnSpeedVariance.x = 1;
    emitter->particleSpawnSpeedVariance.y = 1;
    emitter->particleColor.x = 255;
    emitter->particleColor.y = 255;
    emitter->particleColor.z = 255;
    emitter->particleColor.w = 255;
    emitter->gravity = 0;
    emitter->updateParticleProc = updateParticlePosDefaultProc;
}

void fx_particles_reset(fx_particle_emitter *emitter) {
    emitter->timeSinceLastSpawn = 0;
    for (int i=0; i<emitter->numParticles; i++) {
        emitter->particles[i].age = 0;
        emitter->particles[i].isAlive = 0;
        emitter->particles[i].pos.x = 0;
        emitter->particles[i].pos.y = 0;
        emitter->particles[i].pos.z = 0;
        emitter->particles[i].speed = 0;
    }
}

static void initParticle(fx_particle_emitter *e, fx_particle *p) {
    p->age = 0;
    p->pos = e->pos;
    p->pos.x += fw_math_randf(-e->particleSpawnPosVariance.x,e->particleSpawnPosVariance.x);
    p->pos.y += fw_math_randf(-e->particleSpawnPosVariance.y,e->particleSpawnPosVariance.y);
    p->speed = fw_math_randf(e->particleSpawnSpeedVariance.x,e->particleSpawnSpeedVariance.y);
}

void emitParticle(fx_particle_emitter *e, fw_timer_data *time) {
    e->timeSinceLastSpawn += time->delta;

    if (e->isParticleSpawnEnabled
            && e->timeSinceLastSpawn > e->particleSpawnFreq) {
        e->timeSinceLastSpawn = 0;
        for (int i=0; i<e->numParticles; i++) {
            if (!e->particles[i].isAlive) {
                e->particles[i].isAlive = 1;
                initParticle(e,&e->particles[i]);
                break;
            }
        }
    }
}

void fx_particles_update(fx_particle_emitter *e, fw_timer_data *time) {

    emitParticle(e, time);

    for (int i=0; i<e->numParticles; i++) {
        fx_particle *p = &e->particles[i];

        if (!p->isAlive) {
            continue;
        }

        p->age += time->delta;
        if (p->age > e->particleMaxAge) {
            p->isAlive = 0;
        }

        // Update particle position.
        e->updateParticleProc(p, e, time);

        // Gravity/acceleration.
        p->speed += e->gravity*time->delta;
    }

}

void fx_particles_render(fx_particle_emitter *emitter) {
    if (!emitter->isParticleRenderEnabled) {
        return;
    }

    glColor4ub(emitter->particleColor.x,emitter->particleColor.y,emitter->particleColor.z, emitter->particleColor.w);
    glBegin(GL_POINTS);
    for (int i=0; i<emitter->numParticles; i++) {
        if (emitter->particles[i].isAlive) {
            glVertex3f(emitter->particles[i].pos.x, emitter->particles[i].pos.y, emitter->particles[i].pos.z);
        }
    }
    glEnd();
}
