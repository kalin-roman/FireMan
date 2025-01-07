#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <typeindex>
#include <set>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h>


#include "../Logger/Logger.h"

//#include "../System/Movementsystem.h"

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity
{
private:
    int Id;
public:
    Entity(int Id):Id(Id){};
    Entity(const Entity& entity) = default;
    ~Entity(); 
    void Kill();
    int GetId() const;

    void Tag(const std::string& Tag);
    bool HasTag(const std::string& Tag) const;
    void Group(const std::string& group);
    bool HasGroup(const std::string& group) const;

    Entity& operator =(const Entity& other) = default;
    bool operator > (const Entity other) const {return Id > other.GetId();};
    bool operator < (const Entity other) const {return Id < other.GetId();};
    bool operator == (const Entity other) const {return Id == other.GetId();};
    bool operator != (const Entity other) const {return Id != other.GetId();};

    template<typename TComponent, typename ...TArgs> void AddComponent( TArgs&& ...args);
    template<typename TComponent> void removeComponent();
    template<typename TComponent> bool hasComponent() const;
    template<typename TComponent> TComponent& getComponent() const;

    class Registry* registry;

};

struct IComponent
{
    protected:
        static int nextId;
};

//T = RigidBodyComponent
template<typename T>
class Component: public IComponent {
    public:
        static int GetId() {
            static auto Id = nextId++;
            return Id;
    };
};

class System {
private:
    Signature componentSignature;
    std::vector<Entity> entities;
public:
    System() = default; 
    ~System() = default;

    void AddEntitytoSystem(Entity entity);
    void RemoveEntityfromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentsSignature() const;

    template<typename TComponent> void RequiredComponent();

};

template<typename TComponent>
void System::RequiredComponent(){
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
};

class IPool{
    public:
        virtual ~IPool() = default;
        virtual void RemoveEntityFromPool(int entityId) = 0;
};
////////////////////////////////////////////////////////////////////////
//Pool
////////////////////////////////////////////////////////////////////////
//Just a vector (contiguous data) of object of type T
////////////////////////////////////////////////////////////////////////

template<typename T>
class Pool: public IPool
{
private:
    std::vector<T> data; 
    int size;

    //Helper maps to keep track of entities ids per index, so the vector is always packed
    std::unordered_map<int,int> entityIdToIndex;
    std::unordered_map<int,int> IndexToEntityId;


public:
    Pool(int capacity = 100) {
        size = 0;
        data.resize(capacity);
    };
    virtual ~Pool() = default;

    bool isEmpt() const{
        return size == 0;
    };

    int GetSize() const{
        return size;
    };

    void Resize(int n){
        data.resize(n);
    };

    void Add(T object){
        data.push_back(object);
    };

    void Set(int entityId, T object){
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end()){
            int index = entityIdToIndex[entityId];
            data[index] = object;
        }else{
            int index = size;
            entityIdToIndex.emplace(entityId, index);
            IndexToEntityId.emplace(index, entityId);
            if (index >= data.capacity()){
                data.resize(size * 2);
            }
            data[index] = object;
            size++;
        }
    };

    void Remove(int entityId){
        int indexOfRemoved = entityIdToIndex[entityId];
        int indexOfLast = size - 1;
        data[indexOfRemoved] = data[indexOfLast];

        int entityIdOfLastElement = IndexToEntityId[indexOfLast];
        entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
        IndexToEntityId[indexOfRemoved] = entityIdOfLastElement;

        entityIdToIndex.erase(entityId);
        IndexToEntityId.erase(indexOfLast);

        size--;
    }

    void RemoveEntityFromPool(int entityId) override {
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end()){
            Remove(entityId);
        }
    };


    void Clear(){
        data.clear();
        size = 0;
    };

    T& Get(int entityId){
        int index = entityIdToIndex[entityId];
        return static_cast<T&>(data[index]);
    };

    T& operator [](unsigned int index){
        return data[index];
    }


};

////////////////////////////////////////////////////////////////////////
//Registry
////////////////////////////////////////////////////////////////////////
//The regestry manages the creation and destruction of entities, add system,
//add components
////////////////////////////////////////////////////////////////////////

class Registry
{
private:
    int numOfEntities = 0;

    std::vector<std::shared_ptr <IPool>> componentPools;

    std::vector<Signature> entityComponentSignatures;

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

    void KillEntity(Entity entity);

    void AddEntityToSystem(Entity entity);

    void RemoveEntityFromSystem(Entity entity);

    void TagEntity(Entity entity, const std::string& tag);
    bool EntityHasTag(Entity entity, const std::string& tag) const;
    Entity GetEntityByTag(const std::string& tag) const;
    void RemoveEntityTag(Entity entity);

    void GroupEntity(Entity entity, const std::string& group);
    bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
    std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
    void RemoveEntityGroup(Entity entity);


    //Component managment
    template<typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
    template<typename TComponent> void removeComponent(Entity entity);
    template<typename TComponent> bool hasComponent(Entity entity) const;
    template<typename TComponent> TComponent& getComponent(Entity entity) const;

    //System managment
    template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
    template<typename TSystem> void removeSystem();
    template<typename TSystem> bool hasSystem() const;
    template<typename TSystem> TSystem& GetSystem() const;

};

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

    // Logger::Log("Component id = " + std::to_string(componentId) + " was added to entityID = " + std::to_string(entityId));

    // std::cout << "COMPONENT ID = "  << componentId << "--> POOL SIZE: " << componentPool->GetSize() << std::endl;

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

template<typename TComponent, typename ...TArgs> 
void Entity::AddComponent(TArgs&& ...args){
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
};
template<typename TComponent> 
void Entity::removeComponent(){
    registry->removeComponent<TComponent>(*this);
};
template<typename TComponent> 
bool Entity::hasComponent() const{
    return registry->hasComponent<TComponent>(*this);
};
template<typename TComponent> 
TComponent& Entity::getComponent() const{
    return registry->getComponent<TComponent>(*this);
};

#endif