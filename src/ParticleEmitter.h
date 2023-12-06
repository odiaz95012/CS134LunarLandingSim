#pragma once

#include "TransformObject.h"
#include "ParticleSystem.h"
#include "ofxAssimpModelLoader.h"

typedef enum { DirectionalEmitter, RadialEmitter, RingEmitter } EmitterType;

//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class ParticleEmitter : public TransformObject {
public:
	ParticleEmitter();
	ParticleEmitter(ParticleSystem *s);
	~ParticleEmitter();
	void init();
	void draw();
	void start();
	void stop();
	void setLifespan(const float life)   { lifespan = life; }
	void setVelocity(const ofVec3f &vel) { velocity = vel; }
	void setRate(const float r) { rate = r; }
	void setParticleRadius(const float r) { particleRadius = r; }
	void setEmitterType(EmitterType t) { type = t; }
	void setGroupSize(int s) { groupSize = s; }
	void setOneShot(bool s) { oneShot = s; }
    void setLanderPointer(ofxAssimpModelLoader *model) { lander = model; } // only for lander movement type 
	void update();
    void setRandomLife(bool b) { randomLife = b;  }
    void setParticleColor(ofColor inputColor) { particleColor = inputColor; }
	void spawn(float time);
    void setDamping(float d) { damping = d; }
	ParticleSystem *sys;
	float rate;         // per sec
	bool oneShot;
    float damping;
    float mass;
	bool fired;
    bool randomLife;
    float ringThickness;
	ofVec3f velocity;
    ofVec3f lifeMinMax;
	float lifespan;     // sec
	bool started;
	float lastSpawned;  // ms
	float particleRadius;
	float radius;
	bool visible;
    bool landerSelected; // to allow the dragging of the lander
	int groupSize;      // number of particles to spawn in a group
	bool createdSys;
	EmitterType type;
    ofxAssimpModelLoader *lander;
    ofColor particleColor;
    
    glm::vec3 heading() {
        glm::vec3 initialHeading = glm::vec3(0, 0, 1);    // heading at start
        glm::mat4 Mrot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 1, 0)); // rotate along Y axis
        glm::vec3 h = Mrot * glm::vec4(initialHeading, 1);
        return glm::normalize(h);
    }
    


};
