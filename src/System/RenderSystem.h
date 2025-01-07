#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/Transformcomponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../AssetStore/AssetStore.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>

class RenderSystem : public System {
public:
    RenderSystem(/* args */){
        RequiredComponent<Transformcomponent>();
        RequiredComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<Assetstore>& assetStore, SDL_Rect& camera){

        struct RenderEntity{
            Transformcomponent transformcomponent;
            SpriteComponent spriteComponent;
            AnimationComponent animationComponent;
        };

        std::vector<RenderEntity> Renderentities;

        for(auto entity: GetSystemEntities()){
            RenderEntity rendererEntities;
            rendererEntities.spriteComponent = entity.getComponent<SpriteComponent>();
            rendererEntities.transformcomponent = entity.getComponent<Transformcomponent>();
            rendererEntities.animationComponent = (entity.hasComponent<AnimationComponent>()) ? entity.getComponent<AnimationComponent>() : AnimationComponent(0,0,true,0);
            Renderentities.emplace_back(rendererEntities);

        };
        std::sort(Renderentities.begin(), Renderentities.end(), [&Renderentities](RenderEntity i, RenderEntity j) {
            return (i.spriteComponent.zIndex < j.spriteComponent.zIndex);
        });

        for(auto entity: Renderentities){
            auto transport = entity.transformcomponent;
            auto sprite = entity.spriteComponent;
            auto animation = entity.animationComponent;

            SDL_Rect srcRect = sprite.srcRect;

            SDL_Rect dstRect = {
                static_cast<int>(transport.position.x - (sprite.isFixed ? 0: camera.x)),
                static_cast<int>(transport.position.y - (sprite.isFixed ? 0: camera.y)),
                static_cast<int>(sprite.width * transport.scale.x),
                static_cast<int>(sprite.height * transport.scale.y),

            };


            SDL_RenderCopyEx(
                renderer,
                assetStore->GetTexture(sprite.assetId),
                &srcRect,
                &dstRect,
                transport.rotation,
                NULL,
                // SDL_FLIP_NONE
                animation.flip
            );

        }
    }
};

#endif