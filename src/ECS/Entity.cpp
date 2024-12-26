#include <iostream>
#include "Entity.h"
#include "Registry.h"
#include <algorithm>
#include "../Logger/Logger.h"

int Entity::GetId() const{
    return Id;
};

Entity::~Entity()
{
}
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

