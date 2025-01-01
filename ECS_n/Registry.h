#ifndef REGISTRY_H
#define REGISTRY_H

// #include "System.h"

#include <bitset>
#include <vector>
#include <typeindex>
#include <set>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "Pool.h"
#include "common.h"
#include "System.h"

#include "../Logger/Logger.h"

class Registry
{
private:
    int numOfEntities = 0;

    std::vector<std::shared_ptr <IPool>> componentPools;
    
    std::vector <Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    std::set<Entity> entitiesToBeAdded;

    std::set<Entity> entitiesToBeKilled;

    std::deque<int> freeIds;

    std::unordered_map<std::string, Entity> entityPerTag;
    std::unordered_map<int, std::string> tagPerEntity;

    std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
    std::unordered_map<int, std::string> groupPerEntity;
public:
    Registry(){
        Logger::Log("Registry constructor called! ");
    };
    ~Registry(){
        Logger::Log("Registry destructor called!");
    };
    void Update();
    //Entity managment
    Entity CreateEntity();
    void KillEntity(Entity Entity);
    bool EntityHasTag(Entity entity, const std::string& tag) const;
    void TagEntity(Entity entity, const std::string& tag);
    bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
    void GroupEntity(Entity entity, const std::string& group);

    void AddEntityToSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);

    template<typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
    template<typename TComponent> void removeComponent(Entity entity);
    template<typename TComponent> bool hasComponent(Entity entity) const;
    template<typename TComponent> TComponent& getComponent(Entity entity) const;

    template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
    template<typename TSystem> void removeSystem();
    template<typename TSystem> bool hasSystem() const;
    template<typename TSystem> TSystem& GetSystem() const;


};

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

template<typename TSystem, typename ...TArgs> 
void Registry::AddSystem(TArgs&& ...args){
   std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs> (args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem> 
void Registry::removeSystem(){
    auto itSystem = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(itSystem);
}
template<typename TSystem> 
bool Registry::hasSystem() const {
    return systems.find((std::type_index(typeid(TSystem)))) != systems.end();
}
template<typename TSystem> 
TSystem& Registry::GetSystem() const {
    auto itSystem = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(itSystem->second));
}

#endif