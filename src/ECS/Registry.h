#ifndef REGISTRY_H
#define REGISTRY_H

#include <bitset>
#include <vector>
#include <typeindex>
#include <set>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "../Logger/Logger.h"

class Registry
{
private:
    int numOfEntities = 0;
    
    std::vector <Signature> entityComponentSignatures;

    // std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

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
    // void Update();
    //Entity managment
    Entity CreateEntity();
    void KillEntity(Entity Entity);
    bool EntityHasTag(Entity entity, const std::string& tag) const;
    void TagEntity(Entity entity, const std::string& tag);
    bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
    void GroupEntity(Entity entity, const std::string& group);
    template<typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
    template<typename TComponent> void removeComponent(Entity entity);
    template<typename TComponent> bool hasComponent(Entity entity) const;
    template<typename TComponent> TComponent& getComponent(Entity entity) const;

};

#endif