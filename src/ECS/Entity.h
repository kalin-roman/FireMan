#ifndef ENTITY_H
#define ENTITY_H

#include <bitset>
#include <vector>
#include <typeindex>
#include <set>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "../Logger/Logger.h"


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

#endif