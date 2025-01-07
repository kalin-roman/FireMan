#ifndef WEIGHTSYSTEM_H
#define WEIGHTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/Transformcomponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/WeightComponent.h"
#include "../Components/RigidBodyComponent.h"


#include <SDL2/SDL.h>

class WeightSystem: public System
{
private:
    /* data */
public:
    WeightSystem(/* args */){
        RequiredComponent<SpriteComponent>();
        RequiredComponent<WeightComponent>();
        RequiredComponent<Transformcomponent>();
        RequiredComponent<RigidBodyComponent>();
    };

    bool check() {
        
    }

    void Update(const std::unique_ptr<Registry>& registry, std::vector<std::vector<bool>> map){
        std::vector<Entity> mapEntities = registry->GetEntitiesByGroup("Map");
        std::vector<Entity>::iterator mapEntity = mapEntities.begin();

        auto spriteofMap = mapEntity->getComponent<SpriteComponent>();
        auto transformMaps = mapEntity->getComponent<Transformcomponent>();
        // Logger::Log("wall = " + std::to_string(registry->GetEntitiesByGroup("wall").size()));

        for(auto entity:GetSystemEntities()){
            auto& trans = entity.getComponent<Transformcomponent>();
            auto& weight = entity.getComponent<WeightComponent>();
            auto& rigid = entity.getComponent<RigidBodyComponent>();
            auto& sprite = entity.getComponent<SpriteComponent>();

            int cX = trans.position.x + (sprite.width * trans.scale.x)/2;
            int cY = trans.position.y + (sprite.height * trans.scale.y)/2;

            int pX = cX / (spriteofMap.width * transformMaps.scale.x);
            int pY = cY / (spriteofMap.height * transformMaps.scale.y);

            rigid.velocity.y = weight.velocity;
            if(map[pY + 1][pX]){
                
                rigid.velocity.y = 0.0;
            }
                // for(auto floor : floorEntities){
                //     auto& f = floor.getComponent<Transformcomponent>();
                //     if (
                //         (e.position.x >= f.position.x && e.position.x <= f.position.x + 32) && 
                //         (e.position.y + 32 >= f.position.y && e.position.y + 32 <= f.position.y + 32)) {
                //         // Logger::Log("COLLUSION APPEARS");
                //         rigid.velocity.y = 0.0;
                //         break;
                //     }else{
                //         // Logger::Log("NO COLLUSION APPEARS");
                //         rigid.velocity.y = weight.velocity;
                //     }
                // }
        }
    }
};

#endif