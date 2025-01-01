#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL2/SDL.h>

struct AnimationComponent{
    int numFrams;
    int currentFrame;
    int frameSpeedRate;
    bool  isLoop;
    int offestFrame;
    
    int startTime;

    AnimationComponent(int numFrams = 1, int frameSpeedRate = 1, bool isLoop = true, int offsetFrame = 0){
        this->numFrams = numFrams;
        this->currentFrame = 1;
        this->frameSpeedRate = frameSpeedRate;
        this->isLoop = isLoop;
        this->startTime = SDL_GetTicks();
        this->offestFrame = offestFrame;
    }

};


#endif
