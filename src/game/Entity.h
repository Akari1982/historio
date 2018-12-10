#ifndef ENTITY_H
#define ENTITY_H

#include "EntityTile.h"



class Entity : public EntityTile
{
public:
    Entity();
    virtual ~Entity();

    virtual void Update();
    virtual void UpdateDebug();
};



#endif // ENTITY_H
