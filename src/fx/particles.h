#ifndef FX_PARTICLES_H
#define FX_PARTICLES_H

#include "../fw/types.h"

typedef struct {
    fw_vec3f pos;
    int isAlive;
    float age;
    float speed;
} fx_particle;

typedef struct fx_particle_emitter_struct {
    int isParticleSpawnEnabled;
    int isParticleRenderEnabled;
    fw_vec3f pos;
    float timeSinceLastSpawn;
    float particleMaxAge;
    float particleSpawnFreq;
    fw_vec2f particleSpawnPosVariance;
    fw_vec2f particleSpawnSpeedVariance;
    fw_vec4i particleColor;
    float gravity;
    void (*updateParticleProc)(fx_particle*, struct fx_particle_emitter_struct*, fw_timer_data*);
    fx_particle *particles;
    int numParticles;
} fx_particle_emitter;


void fx_particles_init(fx_particle_emitter *emitter, int numParticles, float particleMaxAge, float particleSpawnFreq);
void fx_particles_reset(fx_particle_emitter *emitter);
void fx_particles_update(fx_particle_emitter *emitter, fw_timer_data *time);
void fx_particles_render(fx_particle_emitter *emitter);


#endif
