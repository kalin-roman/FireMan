#include "System.h"
#include "Component.h"

#include <bitset>
#include <vector>
#include <typeindex>
#include <set>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <algorithm>

#include "../Logger/Logger.h"
#include "Entity.h"

const Signature& System::GetComponentsSignature() const{
    return componentSignature;
};

std::vector<Entity> System::GetSystemEntities() const{
    return entities;
};
void System::AddEntitytoSystem(Entity entity){
    entities.push_back(entity);
};
void System::RemoveEntityfromSystem(Entity entity){
    entities.erase(std::remove_if(entities.begin(), entities.end(),[&entity](Entity other){
        return entity == other;
    }),entities.end());
};