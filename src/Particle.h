#pragma once

#include "ofMain.h"


class Particle {
public:
	Particle();

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
    ofVec3f angVel;
	ofVec3f forces;
    float rotation = 0.0;
    float angularForce = 0.0;
    float angularVelocity = 0.0;
    float angularAcceleration = 0.0;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
	float   birthtime;
	void    integrate();
	void    draw();
	float   age();        // sec
	ofColor color;
    
    glm::vec3 heading() {
        glm::vec3 initialHeading = glm::vec3(0, 0, 1);    // heading at start
        glm::mat4 Mrot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 1, 0)); // rotate along Y axis
        glm::vec3 h = Mrot * glm::vec4(initialHeading, 1);
        return glm::normalize(h);
    }
};


