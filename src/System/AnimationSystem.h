#ifndef AnimationSystem_H
#define AnimationSystem_H

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL2/SDL.h>

class AnimationSystem: public System
{
private:
    /* data */
public:
    AnimationSystem(/* args */){
        RequiredComponent<SpriteComponent>();
        RequiredComponent<AnimationComponent>();
    };

    void Update(){
        for(auto entity:GetSystemEntities()){
            auto& animation = entity.getComponent<AnimationComponent>();
            auto& sprite = entity.getComponent<SpriteComponent>();




            animation.currentFrame = (
                (SDL_GetTicks() - animation.startTime) 
                    * animation.frameSpeedRate/1000) % animation.numFrams;
            
            int pos = animation.currentFrame + animation.offestFrame;
            if (pos > animation.numFrams - 1) {
                animation.currentFrame = pos - animation.numFrams - 1;
            }
                Logger::Log("cF :" + std::to_string(animation.currentFrame));

            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }   
    }

};

#endif