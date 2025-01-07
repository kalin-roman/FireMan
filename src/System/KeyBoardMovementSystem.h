#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyBoardControlComponent.h"
// #include "../Components/ProjectEmitterComponent.h"
#include "../Event/KeyPressedEvent.h"
#include "../Event/KeyUpEvent.h"
#include "../EventBus/EventBus.h"

class KeyBoardMovementSystem: public System
{
private:

public:
    KeyBoardMovementSystem(/* args */){
        RequiredComponent<RigidBodyComponent>();
        RequiredComponent<SpriteComponent>();
        RequiredComponent<KeyBoardControlComponent>();
        // RequiredComponent<ProjectEmitterComponent>();
    };

    void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus){
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyBoardMovementSystem::KeyPressed);
        eventBus->SubscribeToEvent<KeyUpEvent>(this, &KeyBoardMovementSystem::KeyUp);

    }

    void KeyPressed(KeyPressedEvent& event){
        for(auto entity: GetSystemEntities()){
            auto& sprite = entity.getComponent<SpriteComponent>();
            auto& rigid = entity.getComponent<RigidBodyComponent>();
            auto& animation = entity.getComponent<AnimationComponent>();
            const auto keyboardcont = entity.getComponent<KeyBoardControlComponent>();
            animation.numFrams = 6;

            switch (event.key)
            {
            case SDLK_UP:
                rigid.velocity = keyboardcont.upVelocity;
                sprite.srcRect.y = sprite.height * 3;
                break;

            case SDLK_RIGHT:
                rigid.velocity = keyboardcont.rightVelocity;
                sprite.srcRect.y = sprite.height * 10;
                animation.flip = SDL_FLIP_NONE;
                break;

            case SDLK_DOWN:
                rigid.velocity = keyboardcont.downVelocity;
                sprite.srcRect.y = sprite.height * 3;
                break;

            case SDLK_LEFT:
                rigid.velocity = keyboardcont.leftVelocity;
                sprite.srcRect.y = sprite.height * 10;
                animation.flip = SDL_FLIP_HORIZONTAL;
                break;
                        
            default:
                break;
            }

        }

    }

    void KeyUp(KeyUpEvent& event){
         for(auto entity: GetSystemEntities()){
            auto& sprite = entity.getComponent<SpriteComponent>();
            auto& rigid = entity.getComponent<RigidBodyComponent>();
            auto& animation = entity.getComponent<AnimationComponent>();
            const auto keyboardcont = entity.getComponent<KeyBoardControlComponent>();

            rigid.velocity = keyboardcont.idleVelocity;
            animation.numFrams = 4;
            sprite.srcRect.y = sprite.height * 7;
            
        }
    }
    void Update(){

    }
};


#endif

