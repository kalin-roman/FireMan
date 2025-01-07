#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/Transformcomponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/WeightComponent.h"

class Movementsystem : public System {
public:
    Movementsystem(/* args */){
        RequiredComponent<Transformcomponent>();
        RequiredComponent<RigidBodyComponent>();
    }

    void Update(double deltatime){
        for(auto entity: GetSystemEntities()){
            auto& transport = entity.getComponent<Transformcomponent>();
            auto weight = entity.getComponent<WeightComponent>();
            const auto rigid = entity.getComponent<RigidBodyComponent>();

            transport.position.x += rigid.velocity.x * deltatime;
            transport.position.y += rigid.velocity.y * deltatime;

            // Logger::Log(
            //     "Entity id = "+ 
            //     std::to_string(entity.GetId())+
            //     "position is now"+
            //     std::to_string(transport.position.x)+
            //     ','+
            //     std::to_string(transport.position.y)
            // );
             
        }
    }
};

#endif