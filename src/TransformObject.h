//
//  TransformObject.h
//  FinalProject-LunarLandingSim
//
//  Created by Oscar Diaz Vega on 4/24/23.
//

#ifndef TransformObject_h
#define TransformObject_h
#include "ofMain.h"

class TransformObject {
public:
    TransformObject() {
        position = glm::vec3(0, 0, 0);
        scale = glm::vec3(1, 1, 1);
        rotation = 0;
    }
    glm::mat4 getMatrix() {
        glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(position));
        glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
        glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);
        return (trans * rot * scale);
    }
    void setPosition(const ofVec3f & pos) { position = pos;}
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 scale;
    float    rotation;
    bool    bSelected;
};

#endif /* TransformObject_h */
