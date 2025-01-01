#include <iostream>
#include "ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"

int IComponent::nextId = 0;

int Entity::GetId() const{
    return Id;
};

Entity::~Entity()
{
};

void Entity::Kill(){
    registry->KillEntity(*this);
}
void Entity::Tag(const std::string& Tag){
    registry->TagEntity(*this, Tag);
}
bool Entity::HasTag(const std::string& Tag) const{
    return registry->EntityHasTag(*this,Tag);
}
void Entity::Group(const std::string& group){
    registry->GroupEntity(*this,group);
}
bool Entity::HasGroup(const std::string& group) const{
    return registry->EntityBelongsToGroup(*this,group);
}

void System::AddEntitytoSystem(Entity entity){
    entities.push_back(entity);
};
void System::RemoveEntityfromSystem(Entity entity){
    // for (auto it = entities.begin(); it != entities.end();++it){
    //     if (it->GetId() == entity.GetId())
    //     { 
    //         entities.erase(it);
    //         return;
    //     };

    //};
    entities.erase(std::remove_if(entities.begin(), entities.end(),[&entity](Entity other){
        return entity == other;
    }),entities.end());
};
std::vector<Entity> System::GetSystemEntities() const{
    return entities;
};
const Signature& System::GetComponentsSignature() const{
    return componentSignature;
};

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

void Registry::TagEntity(Entity entity, const std::string& tag){
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
};

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const {
    if(tagPerEntity.find(entity.GetId()) == tagPerEntity.end()){
        return false;
    }
    return entityPerTag.find(tag)->second == entity;

}

Entity Registry::GetEntityByTag(const std::string& tag) const {
    return entityPerTag.at(tag);
};

void Registry::RemoveEntityTag(Entity entity){
    auto taggedEntity = tagPerEntity.find(entity.GetId());
    if (taggedEntity != tagPerEntity.end()){
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
};

void Registry::GroupEntity(Entity entity, const std::string& group){
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}
bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const {
    if (entitiesPerGroup.find(group) == entitiesPerGroup.end()){
        return false;
    }
    auto groupPerEntity = entitiesPerGroup.at(group);
    return groupPerEntity.find(entity.GetId()) != groupPerEntity.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const {
    auto& setOfEntities = entitiesPerGroup.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::RemoveEntityGroup(Entity entity) {
    auto groupedEntity = groupPerEntity.find(entity.GetId());
    if (groupedEntity != groupPerEntity.end()){
        auto group = entitiesPerGroup.find(groupedEntity->second);
        if (group != entitiesPerGroup.end()){
            auto entityInGroup = group->second.find(entity);
            if (entityInGroup != group->second.end()){
                group->second.erase(entityInGroup);
            }
        }
        groupPerEntity.erase(groupedEntity);
    }
}

void Registry::Update() {
    for(auto entity: entitiesToBeAdded){
        AddEntityToSystem(entity);
    }
    entitiesToBeAdded.clear();

    for (auto entity: entitiesToBeKilled){
        RemoveEntityFromSystem(entity); 
        entityComponentSignatures[entity.GetId()].reset();

        for(auto pool: componentPools){
            if(pool != NULL){
                pool->RemoveEntityFromPool(entity.GetId());
            }
        }

        freeIds.push_front(entity.GetId());

        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
    }
    entitiesToBeKilled.clear();
}
