#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL2/SDL.h>

struct AnimationComponent{
    int numFrams;
    int currentFrame;
    int frameSpeedRate;
    bool  isLoop;
    int offsetFrame;
    SDL_RendererFlip flip;
    int startTime;

    AnimationComponent(int numFrams = 1, int frameSpeedRate = 1, bool isLoop = true, int offsetFrame = 0, SDL_RendererFlip flip = SDL_FLIP_NONE){
        this->numFrams = numFrams;
        this->currentFrame = 1;
        this->frameSpeedRate = frameSpeedRate;
        this->isLoop = isLoop;
        this->startTime = SDL_GetTicks();
        this->offsetFrame = offsetFrame;
        this->flip = flip;
    }

};


#endif
