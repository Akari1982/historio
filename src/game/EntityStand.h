#ifndef ENTITY_STAND_H
#define ENTITY_STAND_H

#include "Entity.h"



class EntityStand : public Entity
{
public:
    EntityStand();
    virtual ~EntityStand();

    virtual void Update();
    virtual void UpdateDebug();
};



#endif // ENTITY_STAND_H
