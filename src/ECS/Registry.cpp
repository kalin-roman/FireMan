#include "Registry.h"
#include "Entity.h"
#include "Component.h"


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

template<typename TComponent> 
void Registry::removeComponent(Entity entity){
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    std::shared_ptr <Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    componentPool->Remove(entityId);

    entityComponentSignatures[entityId].set(componentId, false);

    Logger::Log("Component id = " + std::to_string(componentId) + " was removed from entityID = " + std::to_string(entityId));

}

template<typename TComponent> 
bool Registry::hasComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent> 
TComponent& Registry::getComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    std::shared_ptr <Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entityId);
}

template <typename TComponent, typename ...TArgs> 
void Registry::AddComponent(Entity entity, TArgs&& ...args){
    const auto componentId = Component<TComponent>::GetId();
    //Logger::Log("cID="+ std::to_string(componentId));
    const auto entityId = entity.GetId();


    if(componentId >= static_cast<int>(componentPools.size())){
        componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId])
    {
        std::shared_ptr <Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }
    
    std::shared_ptr <Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId,newComponent);

    entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Component id = " + std::to_string(componentId) + " was added to entityID = " + std::to_string(entityId));

    std::cout << "COMPONENT ID = "  << componentId << "--> POOL SIZE: " << componentPool->GetSize() << std::endl;

}