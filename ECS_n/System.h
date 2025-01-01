#ifndef SYSTEM_H
#define SYSTEM_H


#include <bitset>
#include <vector>
#include <typeindex>
#include <set>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "Entity.h"
#include "Pool.h"
#include "common.h"


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

#endif