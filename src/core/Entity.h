#ifndef ENTITY_H
#define ENTITY_H

#include "Tile.h"



class Entity : public Tile
{
public:
    Entity();
    virtual ~Entity();

    virtual void Update();
    virtual void UpdateDebug();
};



#endif // ENTITY_H
