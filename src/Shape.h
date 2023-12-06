//
//  Shape.h
//  FinalProject-LunarLandingSim
//
//  Created by Oscar Diaz Vega on 4/27/23.
//

#ifndef Shape_h
#define Shape_h
#include "ofMain.h"

class Shape {
public:
    Shape() {};
    virtual void draw() {};
    virtual bool inside() { return false;  }

    ofVec3f pos = ofVec3f(0,0,0);
    float rotation = 0.0;
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

    // get transformation matrix for object (based on it's current pos, rotation and scale channels)
    glm::mat4 getMatrix() {
        glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
        glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
        glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);

        return (trans * rot * scale);
    }

    glm::vec3 minPoint;
    glm::vec3 maxPoint;
    glm::vec3 boxSize;
    bool bSelected = false;
    bool bCollision = false;
    ofColor color = ofColor::red;

};


#endif /* Shape_h */
