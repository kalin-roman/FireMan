#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include <glm/glm.hpp>

struct KeyBoardControlComponent
{
    glm::vec2 upVelocity;
    glm::vec2 rightVelocity;
    glm::vec2 downVelocity;
    glm::vec2 leftVelocity;
    glm::vec2 idleVelocity;


    KeyBoardControlComponent(glm::vec2 upVelocity = glm::vec2(0), glm::vec2 rightVelocity = glm::vec2(0), glm::vec2 downVelocity = glm::vec2(0),glm::vec2 leftVelocity = glm::vec2(0)){
        this->upVelocity = upVelocity;
        this->rightVelocity = rightVelocity;
        this->downVelocity = downVelocity;
        this->leftVelocity = leftVelocity;
        this->idleVelocity = glm::vec2(0.0,0.0);
    }
};


#endif