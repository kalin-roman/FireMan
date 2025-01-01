#include "Registry.h"
#include "Entity.h"
#include "Component.h"
#include "Pool.h"

void Registry::KillEntity(Entity entity){
    entitiesToBeKilled.insert(entity);
}

Entity Registry::CreateEntity(){
    
    int entityId;

    if(freeIds.empty()){
        entityId = numOfEntities++;
        if (entityId >= static_cast<int>(entityComponentSignatures.size())){
            entityComponentSignatures.resize(numOfEntities + 1);
        }
    }
    else
    {
        entityId = freeIds.front();
        freeIds.pop_front();
    }


    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with an Id = " + std::to_string(entityId));

    return entity;

}

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const {
    if(tagPerEntity.find(entity.GetId()) == tagPerEntity.end()){
        return false;
    }
    return entityPerTag.find(tag)->second == entity;

}
void Registry::TagEntity(Entity entity, const std::string& tag){
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
};

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const {
    if (entitiesPerGroup.find(group) == entitiesPerGroup.end()){
        return false;
    }
    auto groupPerEntity = entitiesPerGroup.at(group);
    return groupPerEntity.find(entity.GetId()) != groupPerEntity.end();
}

void Registry::GroupEntity(Entity entity, const std::string& group){
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

void Registry::RemoveEntityFromSystem(Entity entity){
    for(auto system: systems){
        system.second->RemoveEntityfromSystem(entity);
    }
};
void Registry::AddEntityToSystem(Entity entity){
    const auto entityId = entity.GetId();

    const auto& entitySignature = entityComponentSignatures[entityId];

    for(auto& system: systems){
        const auto& systemSignature = system.second->GetComponentsSignature();

        bool isInter = (entitySignature & systemSignature) == systemSignature;

        if (isInter){
            system.second->AddEntitytoSystem(entity);
        };

    };

}

void Registry::Update() {
    for(auto entity: entitiesToBeAdded){
        AddEntityToSystem(entity);
    }
    entitiesToBeAdded.clear();
}
