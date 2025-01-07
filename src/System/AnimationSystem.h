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


            int l = SDL_GetTicks() - animation.startTime;
            int n = l * animation.frameSpeedRate/1000;     
            animation.currentFrame = (n + animation.offsetFrame) % animation.numFrams;
            
            if (entity.HasGroup("hummer")) {
                
                // Logger::Log("L:" + std::to_string(l) + " n="+ std::to_string(n) + " off=" + std::to_string(animation.offsetFrame) );
                // 
            }   

            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }   
    }

};

#endif