#ifndef WEIGHTCOMPONENT_H
#define WEIGHTCOMPONENT_H

#include <SDL2/SDL.h>

struct WeightComponent{
    float velocity;
    WeightComponent(float velocity = 0){
        this->velocity = velocity;
    }

};


#endif
