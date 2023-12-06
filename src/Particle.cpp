#include "Particle.h"


Particle::Particle() {

    // initialize particle with some reasonable values first;
    //
    velocity.set(0, 0, 0);
    acceleration.set(0, 0, 0);
    position.set(0, 0, 0);
    //rotation.set(0, 0, 0);
    rotation = 0;
    forces.set(0, 0, 0);
    lifespan = 5;
    birthtime = 0;
    radius = .1;
    damping = .99;
    mass = 1;
    color = ofColor::aquamarine;
}

void Particle::draw() {
    ofSetColor(color);
    //ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
    ofDrawSphere(position, radius);
}
void Particle::integrate() {

    
    if(ofGetFrameRate() < 45) return; // for the initial low framerate drop
    
    
    float dt = 1.0 / ofGetFrameRate();

    // update position based on velocity
    position += (velocity * dt);

    // update acceleration with accumulated forces
    ofVec3f accel = acceleration; // start with any acceleration already on the particle
    accel += (forces * (1.0 / mass));
    velocity += accel * dt;

    // add damping
    velocity *= damping;
    
    rotation += (angularVelocity * dt);
    float a = angularAcceleration;
    a += (angularForce * 1.0 / mass);
    angularVelocity += a * dt;
    angularVelocity *= damping;
}

//  return age in seconds
//
float Particle::age() {
    return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}


